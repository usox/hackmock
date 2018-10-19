<?hh // strict
namespace Usox\HackMock;

use type Facebook\HackCodegen\HackCodegenFactory;
use type Facebook\HackCodegen\HackCodegenConfig;
use namespace HH\Lib\{Str};

final class Mock<TC> implements MockInterface {

	private HackCodegenFactory $code_generator;

	public function __construct(private classname<TC> $interface_name): void {
		$this->code_generator = new HackCodegenFactory(new HackCodegenConfig());
	}

	public function build(): TC {
		$rfl = new \ReflectionClass($this->interface_name);

		$mock_name = Str\format(
			'%s_Implementation%s',
			$rfl->getShortName(),
			\spl_object_hash($this)
		);

		if ($rfl->isInterface()) {
			$class = $this->code_generator->codegenClass($mock_name)
				->addInterface(
					$this->code_generator->codegenImplementsInterface($rfl->getName())
				)
				->addEmptyUserAttribute('__MockClass');
		} else {
			$class = $this->code_generator->codegenClass($mock_name)
				->setExtends($rfl->getName())
				->addEmptyUserAttribute('__MockClass');
		}

		foreach ($rfl->getMethods() as $method) {
			$method_name = $method->getName();

			if ($method_name === '__construct') {
				$gen_method = $this
					->code_generator
					->codegenMethod($method_name)
					->setReturnType('void');
				$class->addMethod($gen_method);
				continue;
			}

			$gen_method = $this
				->code_generator
				->codegenMethod($method_name)
				->setReturnType('mixed')
				->setIsStatic($method->isStatic())
				->setBodyf(
					'return \Usox\HackMock\process_expectation(__CLASS__, \'%s\', vec(func_get_args()));',
					$method_name
				);

			foreach ($method->getParameters() as $parameter) {
				$return_type_hint = Str\trim((string) $parameter->getType());

				if ($parameter->isDefaultValueAvailable() === true) {
					if ($parameter->isDefaultValueConstant()) {
						$gen_method->addParameterf(
							'%s $%s = %s',
							(string) $parameter->getType(),
							$parameter->getName(),
							$parameter->getDefaultValue() === null ? 'null' : $parameter->getDefaultValue()
						);
					} else {
						if (Str\contains((string) $parameter->getType(), 'string')) {
							$gen_method->addParameterf(
								'%s $%s = \'%s\'',
								(string) $parameter->getType(),
								$parameter->getName(),
								$parameter->getDefaultValue()
							);
						} else {
							$default_value = $parameter->getDefaultValue();
							$nullable = '';
							if (is_array($default_value)) {
								$default_value = '[]';
							}
							$gen_method->addParameterf(
								'%s%s $%s = %s',
								$return_type_hint,
								$nullable,
								$parameter->getName(),
								$default_value
							);
						}
					}
				} else {
					$nullable = '';
					$default = '';
					if ($parameter->allowsNull()) {
						$default = ' = null';
					}
					$gen_method->addParameterf(
						'%s%s $%s%s',
						$nullable,
						$return_type_hint,
						$parameter->getName(),
						$default
					);
				}
			}

			$class->addMethod($gen_method);
		}

		// UNSAFE
		eval($class->render());

		return new $mock_name();
	}
}

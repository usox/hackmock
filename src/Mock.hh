<?hh // strict
namespace Usox\HackMock;

use Facebook\HackCodegen\CodegenClass;
use Facebook\HackCodegen\HackCodegenFactory;
use Facebook\HackCodegen\HackCodegenConfig;
use HH\Lib\{Str, Dict, C};

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

		$class = $this->code_generator->codegenClass($mock_name)
			->addInterface(
				$this->code_generator->codegenImplementsInterface($rfl->getName())
			)
			->addEmptyUserAttribute('__MockClass');

		foreach ($rfl->getMethods() as $method) {
			$method_name = $method->getName();

			$gen_method = $this
				->code_generator
				->codegenMethod($method_name)
				->setReturnType('mixed')
				->setBodyf(
					'return \Usox\HackMock\processExpectation(__CLASS__, \'%s\', func_get_args());',
					$method_name
				);

			foreach ($method->getParameters() as $parameter) {
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
							$gen_method->addParameterf(
								'%s $%s = %s',
								(string) $parameter->getType(),
								$parameter->getName(),
								$parameter->getDefaultValue()
							);
						}
					}
				} else {
					$gen_method->addParameterf(
						'%s $%s',
						$parameter->getTypehintText(),
						$parameter->getName(),
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
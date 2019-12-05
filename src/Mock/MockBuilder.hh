<?hh // strict

namespace Usox\HackMock\Mock;

use type Facebook\HackCodegen\{CodegenClass, HackCodegenConfig, HackCodegenFactory};
use namespace HH\Lib\Str;

final class MockBuilder {

    private HackCodegenFactory $code_generator;

    private string $mock_name = '';

	private vec<Param\ParamGeneratorInterface> $param_generators = vec[];

	public function __construct(
        private \ReflectionClass $class
    ): void {
		$this->code_generator = new HackCodegenFactory(new HackCodegenConfig());
		$this->param_generators = vec[
			new Param\ParamGeneratorDefaultValueWithConstant(),
			new Param\ParamGeneratorDefaultValue(),
			new Param\ParamGeneratorWithoutDefaultValue()
		];
	}

    public function setName(string $mock_name): this {
        $this->mock_name = $mock_name;

        return $this;
    }

    public function get(): CodegenClass {
		$class = $this->generateClass();

		foreach ($this->class->getMethods() as $method) {
			$method_name = $method->getName();

			if ($method->isConstructor()) {
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
				->setIsStatic($method->isStatic());

			$param_list = varray[];

			foreach ($method->getParameters() as $param) {
				foreach ($this->param_generators as $generator) {
					if ($generator->applies($param)) {
						$generator->generate($param, $gen_method);
					}
				}

				$param_list[] = '$args[] = $'.$param->getName().';';
			}

			$gen_method->setBodyf(
				'$args = vec[]; %s return \Usox\HackMock\process_expectation(__CLASS__, \'%s\', $args);',
				Str\join($param_list, \PHP_EOL),
				$method_name,
			);

			$class->addMethod($gen_method);
		}

        return $class;
    }

	private function generateClass(): CodegenClass {
		if ($this->class->isInterface()) {
            $class = $this->code_generator
                ->codegenClass($this->mock_name)
                ->addInterface(
                    $this->code_generator
                        ->codegenImplementsInterface($this->class->getName()),
                );
		} else {
            $class = $this->code_generator
                ->codegenClass($this->mock_name)
                ->setExtends($this->class->getName());
		}
		$class->addEmptyUserAttribute('__MockClass');

		return $class;
	}
}

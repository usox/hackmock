<?hh // strict

namespace Usox\HackMock\Mock;

use type Facebook\HackCodegen\CodegenClass;
use type Facebook\HackCodegen\HackCodegenFactory;
use type Facebook\HackCodegen\HackCodegenConfig;
use namespace HH\Lib\{Str, Vec};

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

			if ($method->isConstructor() || $method->isDestructor()) {
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

			foreach ($method->getParameters() as $param) {
				foreach ($this->param_generators as $generator) {
					if ($generator->applies($param)) {
						$generator->generate($param, $gen_method);
					}
				}
			}

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
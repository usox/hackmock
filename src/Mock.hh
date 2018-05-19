<?hh // strict
namespace Usox\HackMock;

use Facebook\HackCodegen\CodegenClass;
use Facebook\HackCodegen\HackCodegenFactory;
use Facebook\HackCodegen\HackCodegenConfig;
use HH\Lib\{Str, Dict};

final class Mock<TC> implements MockInterface {

	private Vector<ExpectationInterface> $expectations = Vector{};

	private HackCodegenFactory $code_generator;

	private static Map<string, mixed> $registry = Map{};

	public function __construct(private classname<TC> $interface_name): void {
		$this->code_generator = new HackCodegenFactory(new HackCodegenConfig());
	}

	public function expects(string $method_name): ExpectationInterface {
		$expectation = new Expectation($method_name);
		$this->expectations->add($expectation);
		return $expectation;
	}

	public function build(): TC {
		$rfl = new \ReflectionClass($this->interface_name);

		$mock_name = Str\format(
			'%s_Implementation%s',
			$rfl->getShortName(),
			\spl_object_hash($this)
		);

		$methods = dict($rfl->getMethods());

		$class = $this->code_generator->codegenClass($mock_name)
			->addInterface(
				$this->code_generator->codegenImplementsInterface($rfl->getName())
			)
			->addEmptyUserAttribute('__MockClass');

		foreach ($this->expectations as $expectation) {
			$this->addMethod($class, $expectation);

			$methods = Dict\filter(
				$methods,
				$value ==> $value->getName() !== $expectation->getMethodName()
			);
		}

		$this->addStubsForRemainingMethods($class, $methods);

		// UNSAFE
		eval($class->render());

		return new $mock_name();
	}

	public static function getRegistry(): Map<string, mixed> {
		return static::$registry;
	}

	private function evaluate(string $class): void {

	}

	private function addMethod(CodegenClass $class, ExpectationInterface $expectation): void {
		$class->addMethod(
			$this->code_generator->codegenMethod(
				$expectation->getMethodName()
			)
			->addParameter('mixed ...$params')
			->setReturnType(
				$expectation->getReturnType()
			)
			->setBodyf(
				'return \%s::getRegistry()[\'%s\'];',
				__CLASS__,
				$expectation->getMethodName()
			)
		);
	}

	private function addStubsForRemainingMethods(
		CodegenClass $class,
		dict<int, \ReflectionMethod> $methods
	): void {
		foreach ($methods as $method) {
			$return_type = 'mixed';
			if ($method->hasReturnType()) {
				$return_type = (string) $method->getReturnType();
			}

			$class->addMethod(
				$this->code_generator->codegenMethod(
					$method->getName()
				)
				->addParameter('mixed ...$params')
				->setReturnType($return_type)
				->setBodyf(
					'return;'
				)
			);
		}
	}
}

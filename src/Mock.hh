<?hh // strict
namespace Usox\HackMock;

use Facebook\HackCodegen\HackCodegenFactory;
use Facebook\HackCodegen\HackCodegenConfig;
use Facebook\DefinitionFinder\FileParser;
use HH\Lib\Str;

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

		$path = $rfl->getFileName();
		if ($path === false) {
			throw new \Exception('file not found');
		}

		$parser = FileParser::FromFile(\realpath($path));

		$mock_name = Str\format(
			'%s_Implementation%s',
			$rfl->getShortName(),
			\spl_object_hash($this)
		);

		$methods = [];

		$class = $this->code_generator->codegenClass($mock_name)
			->addInterface(
				$this->code_generator->codegenImplementsInterface($rfl->getName())
			)
			->addEmptyUserAttribute('__MockClass');

		foreach ($this->expectations as $expectation) {
			
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

		$file = $this->code_generator->codegenFile('')
			->addClass($class)
			->render();

		$file = Str\replace($file, '<?hh // strict', '');
		// UNSAFE
		eval($file);

		return new $mock_name();
	}

	public static function getRegistry(): Map<string, mixed> {
		return static::$registry;
	}
}

<?hh // partial
namespace Usox\HackMock;

use Facebook\HackCodegen\HackCodegenFactory;
use Facebook\HackCodegen\HackCodegenConfig;
use Facebook\DefinitionFinder\FileParser;

final class MockBuilder {

	private Vector<ExpectationInterface> $expectations = Vector{};

	private HackCodegenFactory $code_generator;

	public function __construct(): void {
		$this->code_generator = new HackCodegenFactory(new HackCodegenConfig());
	}

	public function prototype<T>(classname<T> $interface_name): MockInterface {
		return new Mock($interface_name);
	}

	public function expects(string $method_name): ExpectationInterface {
		$expectation = new Expectation($method_name);
		$this->expectations->add($expectation);
		return $expectation;
	}
}

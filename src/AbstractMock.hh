<?hh // strict
namespace Usox\HackMock;

abstract class AbstractMock implements MockInterface {
	
	private Vector<ExpectationInterface> $expectations = Vector{};

	public function expects(string $method_name): ExpectationInterface {
		$expectation = new Expectation($method_name);
		$this->expectations->add($expectation);
		return $expectation;
	}

	public function __getExpectations(): Vector<ExpectationInterface> {
		return $this->expectations;
	}
}

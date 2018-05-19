<?hh // strict
namespace Usox\HackMock;

use Facebook\HackCodegen\HackCodegenFactory;
use Facebook\HackCodegen\CodegenMethod;

final class Expectation implements ExpectationInterface {

	private string $return_type = '';

	private mixed $return_value;

	public function __construct(private string $method_name): void {
	}

	public function times(int $times_called): this {
		// TBD implements `times
		return $this;
	}

	public function with(mixed ...$parameters): this {

		// TBD validate parameters
		return $this;
	}

	public function andReturn(mixed $return_value): this {
		$this->return_type = \gettype($return_value);

		$reg = \Usox\HackMock\Mock::getRegistry()[$this->getMethodName()] = $return_value;

		return $this;
	}

	public function getMethodName(): string {
		return $this->method_name;
	}

	public function getReturnType(): string {
		return $this->return_type;
	}
}

<?hh // strict
namespace Usox\HackMock;

use Facebook\HackCodegen\HackCodegenFactory;
use Facebook\HackCodegen\CodegenMethod;

final class Expectation implements ExpectationInterface {

	private mixed $return_value;

	private ?\Throwable $throws;

	public function __construct(private string $method_name): void {
	}

	public function times(int $times_called): this {
		// TODO implement times
		return $this;
	}

	public function with(mixed ...$parameters): this {
		// TODO validate parameters
		return $this;
	}

	public function andReturn(mixed $return_value): this {
		$this->return_value = $return_value;

		return $this;
	}

	public function andThrow(\Throwable $e): this {
		$this->throws = $e;

		return $this;
	}

	public function getMethodName(): string {
		return $this->method_name;
	}

	public function execute(array<mixed> $method_params): mixed {
		if ($this->throws !== null) {
			throw $this->throws;
		}

		return $this->return_value;
	}
}

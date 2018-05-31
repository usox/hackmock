<?hh // strict
namespace Usox\HackMock;

use Facebook\HackCodegen\HackCodegenFactory;
use Facebook\HackCodegen\CodegenMethod;
use function Facebook\FBExpect\expect;
use HH\Lib\C;

final class Expectation implements ExpectationInterface {

	private mixed $return_value = null;

	private vec<mixed> $parameters = vec[];

	private ?\Throwable $throws;

	public function __construct(private string $method_name): void {
	}

	public function times(int $times_called): this {
		// TODO implement times
		return $this;
	}

	public function with(mixed ...$parameters): this {
		$this->parameters = vec($parameters);

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

	public function execute(array<mixed> $method_params): mixed {
		if (C\count($this->parameters) > 0) {
			$this->validateParams(vec($method_params));
		}

		if ($this->throws !== null) {
			throw $this->throws;
		}

		return $this->return_value;
	}

	public function getMethodName(): string {
		return $this->method_name;
	}

	private function validateParams(vec<mixed> $method_params): void {
		expect(
			$method_params
		)
		->toBeSame(
			$this->parameters
		);
	}
}

<?hh // strict
namespace Usox\HackMock;

use namespace HH\Lib\{C, Str};

final class Expectation implements ExpectationInterface {

	private mixed $return_value = null;

	private ?int $times_to_call;

	private int $call_counter = 0;

	private vec<mixed> $parameters = vec[];

	private ?\Throwable $throws;

	public function __construct(private string $method_name): void {
	}

	public function times(int $times_to_call): this {
		$this->times_to_call = $times_to_call;

		return $this;
	}

	public function once(): this {
		$this->times(1);

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

	public function execute(vec<mixed> $method_params): mixed {
		$this->call_counter++;

		if (C\count($this->parameters) > 0) {
			$this->validateParams($method_params);
		}

		if ($this->times_to_call !== null) {
			if ($this->times_to_call < $this->call_counter) {
				clear_global_state();

				throw new Exception\MethodCallCountException(
					Str\format(
						'Method `%s` is not expected to be called more then %d times',
						$this->method_name,
						(int) $this->times_to_call
					)
				);
			}
		}

		if ($this->throws !== null) {
			throw $this->throws;
		}

		return $this->return_value;
	}

	public function getMethodName(): string {
		return $this->method_name;
	}

	public function isActive(): bool {
		if ($this->times_to_call === null) {
			return false;
		}

		return $this->times_to_call > $this->call_counter;
	}

	public function validateParams(vec<mixed> $method_params): void {
		foreach ($method_params as $key => $param) {

			$param_expectation = $this->parameters[$key];
			if (\is_callable($param_expectation)) {
				invariant(
					\call_user_func($param_expectation, $param),
					'Parameter validation failed'
				);
			} else {
				invariant(
					$param === $param_expectation,
					'Parameter validation failed'
				);
			}
		}
	}

	public function getParameters(): vec<mixed> {
		return $this->parameters;
	}
}

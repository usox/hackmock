<?hh // strict
namespace Usox\HackMock;

interface ExpectationInterface {

	public function times(int $times_called): this;

	public function once(): this;

	public function with(mixed ...$parameters): this;

	public function andReturn(mixed $return_value): this;

	public function andThrow(\Throwable $e): this;

	public function execute(vec<mixed> $method_params): mixed;

	public function getMethodName(): string;

	public function isActive(): bool;

	public function validateParams(vec<mixed> $params): void;

	public function getParameters(): vec<mixed>;
}

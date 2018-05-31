<?hh // strict

interface SampleInterface {

	public function noParamsAndVoid(): void;

	public function noParamsAndVoidButThrows(): void;

	public function noParamsButReturnsInt(): int;

	public function noParamsButReturnsString(): string;

	public function noParamsButReturnsSampleInterfaceInstance(): SampleInterface;

	public function basicParamValidation(int $int, string $string, float $float, \stdClass $class): mixed;
}
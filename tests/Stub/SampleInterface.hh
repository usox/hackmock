<?hh // strict

namespace Stub;

interface SampleInterface {

	public function noParamsAndVoid(): void;

	public function noParamsAndVoidButThrows(): void;

	public function noParamsButReturnsInt(): int;

	public function noParamsButReturnsString(): string;

	public function noParamsButReturnsSampleInterfaceInstance(): SampleInterface;

	public function basicParamValidation(int $int, string $string, float $float, \stdClass $class): mixed;

	public function paramValidationWithClosure(string $string): mixed;

	public function hasDefaultForParameterBuildsCorrectMock(string $text = 'something'): void;

	public function hasNullAsDefaultForParemterBuildsCorrectMock(?string $text = null): void;

	public function isNullableWithoutDefaultsBuildsCorrectMock(?string $text): void;
}
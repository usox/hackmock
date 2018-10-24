<?hh // strict

namespace Stub;
	
class SampleBaseClass {

	public function __construct(): void {
		throw new \Exception('Should never be thrown');
	}
	
	public function noParamsAndVoid(): void {

	}

	public function intParamAndReturnsInt(int $some_int): int {
		return $some_int;
	}

	public function intOrNullWithDefault(?int $some_int = null): void {
	}

	public static function someStaticFunction(): void {

	}
}
<?hh // strict
	
class SampleBaseClass {
	
	public function noParamsAndVoid(): void {

	}

	public function intParamAndReturnsInt(int $some_int): int {
		return $some_int;
	}

}
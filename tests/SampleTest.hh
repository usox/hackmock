<?hh // strict

use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect};

class SampleTest extends \PHPUnit_Framework_TestCase {

	public function testNoParamsAndVoid(): void {
		$sample = mock(SampleInterface::class);
		
		prospect($sample, 'noParamsAndVoid');

		$sample->noParamsAndVoid();
	}

	public function testNoParamsAndVoidButThrows(): void {
		$sample = mock(SampleInterface::class);

		$message = 'some-throwable-message';
		
		prospect($sample, 'noParamsAndVoidButThrows')
			->andThrow(new \Exception($message));
		
		expect(
			function() use ($sample) {
				$sample->noParamsAndVoidButThrows();
			}
		)
		->toThrow(
			\Exception::class,
			$message
		);
	}

	public function testNoParamsButReturnsInt(): void {
		$sample = mock(SampleInterface::class);

		$return_value = 666;
		
		prospect($sample, 'noParamsButReturnsInt')
			->andReturn($return_value);
		
		expect(
			$sample->noParamsButReturnsInt()
		)
		->toBeSame($return_value);
	}

	public function testNoParamsButReturnsString(): void {
		$sample = mock(SampleInterface::class);

		$return_value = 'some-return-value';
		
		prospect($sample, 'noParamsButReturnsString')
			->andReturn($return_value);
		
		expect(
			$sample->noParamsButReturnsString()
		)
		->toBeSame($return_value);
	}

	public function testNoParamsButReturnsSampleInterfaceInstance(): void {
		$sample = mock(SampleInterface::class);

		$return_value = 'some-return-value';
		
		prospect($sample, 'noParamsButReturnsSampleInterfaceInstance')
			->andReturn(mock(SampleInterface::class));
		
		expect(
			$sample->noParamsButReturnsSampleInterfaceInstance()
		)
		->toBeInstanceOf(SampleInterface::class);
	}

	public function testParamsValidationSucceeds(): void {
		$sample = mock(SampleInterface::class);
		$int = 1234;
		$string = 'string';
		$float = 1.23;
		$class = new \stdClass();

		prospect($sample, 'basicParamValidation')
			->with($int, $string, $float, $class)
			->andReturn(null);

		expect(
			$sample->basicParamValidation($int, $string, $float, $class)
		)
		->toBeNull();
	}

	public function testParamsValidationFails(): void {
		$sample = mock(SampleInterface::class);

		prospect($sample, 'basicParamValidation')
			->with(1234, 'string', 6.66)
			->andReturn(null);

		expect(
			function() use ($sample) {
				$sample->basicParamValidation(5678, 'nostring', 4.2, new \stdClass());
			}
		)
		->toThrow(\Exception::class);
	}
}
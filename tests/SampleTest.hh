<?hh // strict

use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect, tearDown};

class SampleTest extends \PHPUnit_Framework_TestCase {

	public function testNoParamsAndVoid(): void {
		$sample = mock(SampleInterface::class);
		
		prospect($sample, 'noParamsAndVoid')
			->times(1);

		$sample->noParamsAndVoid();

		tearDown();
	}

	public function testNoParamsAndVoidButThrows(): void {
		$sample = mock(SampleInterface::class);

		$message = 'some-throwable-message';
		
		prospect($sample, 'noParamsAndVoidButThrows')
			->times(1)
			->andThrow(new \Exception($message));
		
		expect(
			() ==> $sample->noParamsAndVoidButThrows()
		)
		->toThrow(
			\Exception::class,
			$message
		);

		tearDown();
	}

	public function testNoParamsButReturnsInt(): void {
		$sample = mock(SampleInterface::class);

		$return_value = 666;
		
		prospect($sample, 'noParamsButReturnsInt')
			->times(1)
			->andReturn($return_value);
		
		expect(
			$sample->noParamsButReturnsInt()
		)
		->toBeSame($return_value);

		tearDown();
	}

	public function testNoParamsButReturnsString(): void {
		$sample = mock(SampleInterface::class);

		$return_value = 'some-return-value';
		
		prospect($sample, 'noParamsButReturnsString')
			->times(1)
			->andReturn($return_value);
		
		expect(
			$sample->noParamsButReturnsString()
		)
		->toBeSame($return_value);

		tearDown();
	}

	public function testNoParamsButReturnsSampleInterfaceInstance(): void {
		$sample = mock(SampleInterface::class);

		$return_value = 'some-return-value';

		prospect($sample, 'noParamsButReturnsSampleInterfaceInstance')
			->times(1)
			->andReturn(mock(SampleInterface::class));
		
		expect(
			$sample->noParamsButReturnsSampleInterfaceInstance()
		)
		->toBeInstanceOf(SampleInterface::class);

		tearDown();
	}

	public function testParamsValidationSucceeds(): void {
		$sample = mock(SampleInterface::class);
		$int = 1234;
		$string = 'string';
		$float = 1.23;
		$class = new \stdClass();

		prospect($sample, 'basicParamValidation')
			->with($int, $string, $float, $class)
			->times(1)
			->andReturn(null);

		expect(
			$sample->basicParamValidation($int, $string, $float, $class)
		)
		->toBeNull();

		tearDown();
	}

	public function testParamsValidationFails(): void {
		$sample = mock(SampleInterface::class);

		prospect($sample, 'basicParamValidation')
			->with(1234, 'string', 6.66)
			->times(1)
			->andReturn(null);

		expect(
			() ==> $sample->basicParamValidation(5678, 'nostring', 4.2, new \stdClass())
		)
		->toThrow(\Exception::class);

		tearDown();
	}

	public function testMissingMethodCall(): void {
		$sample = mock(SampleInterface::class);

		$return_value = 'some-return-value';
		
		prospect($sample, 'noParamsButReturnsString')
			->times(1)
			->andReturn($return_value);

		expect(() ==> tearDown())
		->toThrow(
			Usox\HackMock\Exception\MissingMethodCallException::class,
			'Expected method call `noParamsButReturnsString`'
		);
	}

	public function testHavingMoreCallsThenExpectationsThrowsException(): void {
		$sample = mock(SampleInterface::class);

		$return_value = 'some-return-value';

		prospect($sample, 'noParamsButReturnsString')
			->times(1)
			->andReturn($return_value);

		expect(() ==> {
				$sample->noParamsButReturnsString();
				$sample->noParamsButReturnsString();
			}
		)
		->toThrow(
			Usox\HackMock\Exception\MethodCallCountException::class,
			'Method `noParamsButReturnsString` is not expected to be called more then 1 times'
		);

		tearDown();
	}
}
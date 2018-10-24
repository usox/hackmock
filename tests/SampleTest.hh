<?hh // strict

use HH\Lib\Str;
use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect, tear_down};
use Usox\HackMock\Exception\{MissingMethodCallException, UnexpectedMethodCallException};

class SampleTest extends \Usox\HackMock\HackMock {

	public function testNoParamsAndVoid(): void {
		$sample = mock(Stub\SampleInterface::class);
		
		prospect($sample, 'noParamsAndVoid')
			->times(1);

		$sample->noParamsAndVoid();
	}

	public function testNoParamsAndVoidButThrows(): void {
		$sample = mock(Stub\SampleInterface::class);

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
	}

	public function testNoParamsButReturnsInt(): void {
		$sample = mock(Stub\SampleInterface::class);

		$return_value = 666;
		
		prospect($sample, 'noParamsButReturnsInt')
			->times(1)
			->andReturn($return_value);
		
		expect(
			$sample->noParamsButReturnsInt()
		)
		->toBeSame($return_value);
	}

	public function testNoParamsButReturnsString(): void {
		$sample = mock(Stub\SampleInterface::class);

		$return_value = 'some-return-value';
		
		prospect($sample, 'noParamsButReturnsString')
			->times(1)
			->andReturn($return_value);
		
		expect(
			$sample->noParamsButReturnsString()
		)
		->toBeSame($return_value);
	}

	public function testNoParamsButReturnsSampleInterfaceInstance(): void {
		$sample = mock(Stub\SampleInterface::class);

		$return_value = 'some-return-value';

		prospect($sample, 'noParamsButReturnsSampleInterfaceInstance')
			->times(1)
			->andReturn(mock(Stub\SampleInterface::class));
		
		expect(
			$sample->noParamsButReturnsSampleInterfaceInstance()
		)
		->toBeInstanceOf(Stub\SampleInterface::class);
	}

	public function testParamsValidationSucceeds(): void {
		$sample = mock(Stub\SampleInterface::class);
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
	}

	public function testParamsValidationSucceedsTwoTimesInRow(): void {
		$sample = mock(Stub\SampleInterface::class);
		$int = 1234;
		$string = 'string';
		$float = 1.23;
		$class = new \stdClass();

		prospect($sample, 'basicParamValidation')
			->with($int, $string, $float, $class)
			->times(1)
			->andReturn(null);
		prospect($sample, 'basicParamValidation')
			->with($int, $string, $float, $class)
			->times(1)
			->andReturn(null);

		$sample->basicParamValidation($int, $string, $float, $class);
		$sample->basicParamValidation($int, $string, $float, $class);
	}

	public function testParamsValidationSucceedsWithDifferentCallCounts(): void {
		$sample = mock(Stub\SampleInterface::class);
		$int = 1234;
		$string = 'string';
		$float = 1.23;
		$class = new \stdClass();

		prospect($sample, 'basicParamValidation')
			->with($int, $string, $float, $class)
			->times(2)
			->andReturn(null);
		prospect($sample, 'basicParamValidation')
			->with($int, $string, $float, $class)
			->times(1)
			->andReturn(null);

		$sample->basicParamValidation($int, $string, $float, $class);
		$sample->basicParamValidation($int, $string, $float, $class);
		$sample->basicParamValidation($int, $string, $float, $class);
	}

	public function testParamsValidationFailesWithDifferentCallCountsButOneWrongParameter(): void {
		$sample = mock(Stub\SampleInterface::class);
		$int = 1234;
		$string = 'string';
		$float = 1.23;
		$class = new \stdClass();

		prospect($sample, 'basicParamValidation')
			->with($int, $string, $float, $class)
			->times(2)
			->andReturn(null);
		prospect($sample, 'basicParamValidation')
			->with($int, $string, $float, $class)
			->times(1)
			->andReturn(null);

		expect(
			() ==> {
				$sample->basicParamValidation($int, $string, $float, $class);
				$sample->basicParamValidation($int, $string, $float, $class);
				$sample->basicParamValidation($int, $string, 6.66, $class);
			}
		)
		->toThrow(
			UnexpectedMethodCallException::class,
			Str\format(
				'No expectation defined for `%s::basicParamValidation` with parameter `1234,string,6.66,stdClass`',
				\get_class($sample)
			)
		);
	}

	public function testParameterValidationWithClosure(): void {
		$string = 'some-string';

		$sample = mock(Stub\SampleInterface::class);
		prospect($sample, 'paramValidationWithClosure')
			->with((string $value) ==> $string === $string)
			->times(1)
			->andReturn(null);

		expect(
			$sample->paramValidationWithClosure($string)
		)
		->toBeNull();
	}

	public function testParamsValidationFails(): void {
		$sample = mock(Stub\SampleInterface::class);

		prospect($sample, 'basicParamValidation')
			->with(1234, 'string', 6.66)
			->times(1)
			->andReturn(null);

		expect(
			() ==> $sample->basicParamValidation(5678, 'nostring', 4.2, new \stdClass())
		)
		->toThrow(
			UnexpectedMethodCallException::class,
			Str\format('No expectation defined for `%s::basicParamValidation` with parameter `5678,nostring,4.2,stdClass`', \get_class($sample))
		);
	}

	public function testMissingMethodCall(): void {
		$sample = mock(Stub\SampleInterface::class);

		$return_value = 'some-return-value';
		
		prospect($sample, 'noParamsButReturnsString')
			->times(1)
			->andReturn($return_value);

		expect(() ==> tear_down())
		->toThrow(
			MissingMethodCallException::class,
			'Expected method call `noParamsButReturnsString` with parameters ``'
		);
	}

	public function testHavingMoreCallsThenExpectationsThrowsException(): void {
		$sample = mock(Stub\SampleInterface::class);

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
			Usox\HackMock\Exception\UnexpectedMethodCallException::class
		);
	}

	public function testNoParamsAndVoidOnBaseClass(): void {
		$sample = mock(Stub\SampleBaseClass::class);
		
		prospect($sample, 'noParamsAndVoid')
			->times(1);

		$sample->noParamsAndVoid();
	}

	public function testIntParamAndIntReturnValueOnBaseClass(): void {
		$sample = mock(Stub\SampleBaseClass::class);

		$int1 = 666;
		$int2 = 777;
		
		prospect($sample, 'intParamAndReturnsInt')
			->with($int1)
			->times(1)
			->andReturn($int2);

		expect(
			$sample->intParamAndReturnsInt($int1)
		)
		->toBeSame($int2);
	}

	public function testIntOrNullWithDefaultOnBaseClass(): void {
		$sample = mock(Stub\SampleBaseClass::class);

		prospect($sample, 'intOrNullWithDefault')
			->with(55)
			->once();

		$sample->intOrNullWithDefault(55);
	}

	public function testDuplicateExpectationWorks(): void {
		$sample = mock(Stub\SampleBaseClass::class);

		prospect($sample, 'intOrNullWithDefault')
			->with(55)
			->once();
		prospect($sample, 'intOrNullWithDefault')
			->with(55)
			->once();

		$sample->intOrNullWithDefault(55);
		$sample->intOrNullWithDefault(55);
	}
}
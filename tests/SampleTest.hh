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
}
<?hh // strict

use function Usox\HackMock\{mock, prospect};

class UntypedSampleTest extends \Usox\HackMock\HackMock {
    public function testUntypedParameter(): void {
        $sample = mock(UntypedSampleBaseClass::class);

        prospect($sample, 'untypedParameter')->execute(vec[]);
    }

    public function testUntypedDefaultParameter(): void {
        $sample = mock(UntypedSampleBaseClass::class);

        prospect($sample, 'untypedDefaultParameter')->execute(vec[]);
    }
}

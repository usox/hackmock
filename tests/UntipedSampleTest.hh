<?hh // strict

use HH\Lib\Str;
use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect, tear_down};
use Usox\HackMock\Exception\{MissingMethodCallException, UnexpectedMethodCallException};

class UntipedSampleTest extends \Usox\HackMock\HackMock {
    public function testUntipedParameter(): void {
        $sample = mock(UntipedSampleBaseClass::class);

        prospect($sample, 'untipedParameter')->execute(vec[]);
    }

    public function testUntipedDefaultParameter(): void {
        $sample = mock(UntipedSampleBaseClass::class);

        prospect($sample, 'untipedDefaultParameter')->execute(vec[]);
    }
}

<?hh // strict

use function Usox\HackMock\{mock, prospect};

class DefaultValueTest extends \Usox\HackMock\HackMock {
    public function testDefaultBooleanFalse(): void {
        $mock = mock(Stub\DefaultValueClass::class);

        prospect($mock, 'defaultBooleanFalse')->execute(vec[]);
    }

    public function testDefaultBooleanTrue(): void {
        $mock = mock(Stub\DefaultValueClass::class);

        prospect($mock, 'defaultBooleanTrue')->execute(vec[]);
    }

    public function testDefaultNullString(): void {
        $mock = mock(Stub\DefaultValueClass::class);

        prospect($mock, 'defaultNullString')->execute(vec[]);
    }

    public function testDefaultNullArray(): void {
        $mock = mock(Stub\DefaultValueClass::class);

        prospect($mock, 'defaultNullArray')->execute(vec[]);
    }

    public function testDefaultConstantInt(): void {
        $mock = mock(Stub\DefaultValueClass::class);

        prospect($mock, 'defaultConstantInt')->execute(vec[]);
    }

    public function testDefaultConstantString(): void {
        $mock = mock(Stub\DefaultValueClass::class);

        prospect($mock, 'defaultConstantString')->execute(vec[]);
    }

    public function testDefaultConstantArraykey(): void {
        $mock = mock(Stub\DefaultValueClass::class);

        prospect($mock, 'defaultConstantArraykey')->execute(vec[]);
    }
}

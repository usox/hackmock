<?hh // strict

namespace Stub;

/**
 * Version 1.0.1 just fails with boolean values (#3).
 * TODO: add more tests cases?.
 */
class DefaultValueClass {
    const int FOO = 0;
    const string BAR = 'bar';
    const arraykey BAZ = 1;

    public function defaultBooleanFalse(bool $param = false): void {

    }

    public function defaultBooleanTrue(bool $param = true): void {

    }

    public function defaultNullString(?string $param = null): void {

    }

    public function defaultNullArray(?array<arraykey, mixed> $param = null): void {

    }

    public function defaultConstantInt(int $param = self::FOO): void {

    }

    public function defaultConstantString(string $param = self::BAR): void {

    }

    public function defaultConstantArraykey(arraykey $param = self::BAZ): void {

    }
}

[![Build Status](https://travis-ci.org/usox/hackmock.svg?branch=master)](https://travis-ci.org/usox/hackmock)

# HackMock

Creating mock objects for hacklang - yes, seriously.

## Note

Due to the use of `eval`, hackmock may stop working in future hhvm versions.

## What works?
- Strict mode
- Creating mocks of interfaces and concrete classes
- Defining basic method expectations (parameter validation, return value definition)

## What does not work?
- Everything else, especially rare and/or untested cases involving generics, etc.

```php
use function Usox\HackMock\{mock, prospect};

class SomethingTest extends \Usox\HackMock\HackMock {

  public function testSomething() {
    $my_fine_class = mock(SomeInterface::class);

    prospect($my_fine_class, 'someMethodName')
      ->once()
      ->andReturn('some-fine-value');

    prospect($my_fine_class, 'someOtherMethodName')
      ->andThrow(new \Exception('foobar'));
  }
}
```
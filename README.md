[![Build Status](https://travis-ci.org/usox/hackmock.svg?branch=master)](https://travis-ci.org/usox/hackmock)

# HackMock - in development...

Creating mock objects for hacklang - yes, seriously.

It's strongly recommended not to use this in any sort of production environment.

## What works?
- Strict mode
- Creating mocks of interfaces
- Defining method expectations (just the return value and throwing Throwables)

## What does not work?
- Everything else.

## But I want to try
Ok.

```php
use Usox\HackMock\{mock, prospect};

$my_fine_class = mock(SomeInterface::class);

prospect($my_fine_class, 'someMethodName)
  ->times(1)
  ->andReturn('some-fine-value');

prospect($my_fine_class, 'someOtherMethodName')
  ->andThrow(new \Exception('foobar'));
```

## No testlistener support
So don't forget to call `Usox\HackMock\tearDown()` after every test (just use the `tearDown` functionality of phpunit).
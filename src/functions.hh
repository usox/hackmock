<?hh // strict
namespace Usox\HackMock;

use HH\Lib\{C, Str};

function mock<Tcn>(classname<Tcn> $class_name): Tcn {
	return new Mock($class_name)->build();
}

function prospect<TC>(TC $class, string $method_name): ExpectationInterface {
	$expectation = new Expectation($method_name);
	
	globalState()[\get_class($class) . '_' . $method_name] = $expectation;

	return $expectation;
}

function globalState(): Map<string, ExpectationInterface> {
	static $global_state = null;
	if ($global_state === null) {
		$global_state = Map{};
	}

	return $global_state;
}

function processExpectation(
	string $mock_class_name,
	string $method_name,
	array<mixed> $params
): mixed {
	$expectation = globalState()[$mock_class_name . '_' . $method_name] ?? null;
	if ($expectation === null) {
		return null;
	}

	globalState()->remove($mock_class_name . '_' . $method_name);

	return $expectation->execute($params);
}

function tearDown(): void {
	if (C\count(globalState()) > 0) {
		$state = C\first(globalState());

		throw new Exception\MissingMethodCallException(
			Str\format(
				'Expected method call `%s`',
				(string) $state?->getMethodName()
			)
		);
	} 
}
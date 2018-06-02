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
	$state = globalState();
	$identifier = Str\format(
		'%s_%s',
		$mock_class_name,
		$method_name
	);

	$expectation = $state[$identifier] ?? null;
	if ($expectation === null) {
		throw new Exception\UnexpectedMethodCallException(
			Str\format(
				'No expectation defnied for `%s::%s`',
				$mock_class_name,
				$method_name
			)
		);
	}

	return $expectation->execute(vec($params));
}

function tearDown(): void {
	$state = C\find(
		globalState(),
		($expectation) ==> {
			return $expectation->isActive();
		}
	);

	clearGlobalState();

	if ($state !== null) {
		throw new Exception\MissingMethodCallException(
			Str\format(
				'Expected method call `%s`',
				$state->getMethodName()
			)
		);
	}
}

function clearGlobalState(): void {
	globalState()->clear();
}
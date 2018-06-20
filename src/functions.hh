<?hh // strict
namespace Usox\HackMock;

use HH\Lib\{C, Str, Vec};

function mock<Tcn>(classname<Tcn> $class_name): Tcn {
	return (new Mock($class_name))->build();
}

function prospect<TC>(TC $class, string $method_name): ExpectationInterface {
	$expectation = new Expectation($method_name);

	$identifier = Str\format('%s_%s', \get_class($class), $method_name);

	if (globalState() |> C\contains_key($$, $identifier)) {
		globalState()[\get_class($class) . '_' . $method_name][] = $expectation;
	} else {
		globalState()[\get_class($class) . '_' . $method_name] = vec[$expectation];
	}

	return $expectation;
}

function globalState(): Map<string, vec<ExpectationInterface>> {
	static $global_state = null;
	if ($global_state === null) {
		$global_state = Map{};
	}

	return $global_state;
}

function processExpectation(
	string $mock_class_name,
	string $method_name,
	vec<mixed> $params
): mixed {
	$state = globalState();
	$identifier = Str\format(
		'%s_%s',
		$mock_class_name,
		$method_name
	);

	$expectations = $state[$identifier] ?? null;
	if ($expectations === null) {
		clearGlobalState();

		throw new Exception\UnexpectedMethodCallException(
			Str\format(
				'No expectation defined for `%s::%s`',
				$mock_class_name,
				$method_name
			)
		);
	}

	foreach ($expectations as $key => $expectation) {
		try {
			if ($expectation->isActive() === false) {
				continue;
			}
			$expectation->validateParams(vec($params));

			return $expectation->execute(vec($params));
		} catch (InvariantException $e) {
			continue;
		}
	}

	clearGlobalState();

	throw new Exception\UnexpectedMethodCallException(
		vec_to_string($params) |> Str\format(
			'No expectation defined for `%s::%s` with parameter `%s`',
			$mock_class_name,
			$method_name,
			$$
		)
	);
}

function vec_to_string(vec<mixed> $data): string {
		return Vec\map(
			$data,
			(mixed $param) ==> {
				if (\is_object($param)) {
					return \get_class($param);
				}
				if (\is_array($param)) {
					return vec_to_string(vec($param));
				}
				return (string) $param;
			}
		)
		|> Str\join($$, ',');
}

function tearDown(): void {
	$state = C\find(
		globalState(),
		(vec<ExpectationInterface> $expectations) ==> {
			foreach ($expectations as $expectation) {
				return $expectation->isActive();
			}
			return false;
		}
	);

	clearGlobalState();

	if ($state !== null) {
		$expectation = C\first($state);
		if ($expectation !== null) {
			throw new Exception\MissingMethodCallException(
				vec_to_string($expectation->getParameters()) |> Str\format(
					'Expected method call `%s` with parameters `%s`',
					$expectation->getMethodName(),
					$$
				)
			);
		}
	}
}

function clearGlobalState(): void {
	globalState()->clear();
}
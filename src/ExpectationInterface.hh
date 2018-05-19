<?hh // strict
namespace Usox\HackMock;

use Facebook\HackCodegen\HackCodegenFactory;
use Facebook\HackCodegen\CodegenMethod;

interface ExpectationInterface {

	public function times(int $times_called): this;

	public function with(mixed ...$parameters): this;

	public function andReturn(mixed $return_value): this;

	public function getMethodName(): string;

	public function getReturnType(): string;

	public function getReturnValue(): string;
}

<?hh // strict

namespace Usox\HackMock\Mock\Param;

use type Facebook\HackCodegen\CodegenMethod;

interface ParamGeneratorInterface {

    public function applies(\ReflectionParameter $param): bool;

    public function generate(\ReflectionParameter $param, CodegenMethod $method): void;
}
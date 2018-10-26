<?hh // strict

namespace Usox\HackMock\Mock\Param;

use type Facebook\HackCodegen\CodegenMethod;
use namespace HH\Lib\Str;

final class ParamGeneratorWithoutDefaultValue implements ParamGeneratorInterface {

    public function applies(\ReflectionParameter $param): bool {
        return !$param->isDefaultValueAvailable();
    }

    public function generate(\ReflectionParameter $param, CodegenMethod $method): void {
        $default = '';
        if ($param->allowsNull()) {
            // `null` must be defined as default in case, the param is nullable
            $default = ' = null';
        }
        $method->addParameterf(
            '%s $%s%s',
            Str\trim((string) $param->getType()),
            $param->getName(),
            $default,
        );
    }
}
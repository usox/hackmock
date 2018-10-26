<?hh // strict

namespace Usox\HackMock\Mock\Param;

use type Facebook\HackCodegen\CodegenMethod;
use namespace HH\Lib\Str;

final class ParamGeneratorDefaultValue implements ParamGeneratorInterface {

    public function applies(\ReflectionParameter $param): bool {
        return $param->isDefaultValueAvailable() &&
            !$param->isDefaultValueConstant();
    }

    public function generate(\ReflectionParameter $param, CodegenMethod $method): void {
        $param_type_hint = Str\trim((string) $param->getType());
        if (Str\contains($param_type_hint, 'string')) {
            $method->addParameterf(
                '%s $%s = \'%s\'',
                $param_type_hint,
                $param->getName(),
                $param->getDefaultValue(),
            );
        } else {
            $default_value = $param->getDefaultValue();
            if (\is_array($default_value)) {
                $default_value = '[]';
            }
            $method->addParameterf(
                '%s $%s = %s',
                $param_type_hint,
                $param->getName(),
                $default_value,
            );
        }
    }
}
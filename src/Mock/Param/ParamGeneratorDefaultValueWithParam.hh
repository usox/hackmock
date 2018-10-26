<?hh // strict

namespace Usox\HackMock\Mock\Param;

use type Facebook\HackCodegen\CodegenMethod;

final class ParamGeneratorDefaultValueWithConstant implements ParamGeneratorInterface {

    public function applies(\ReflectionParameter $param): bool {
        return $param->isDefaultValueAvailable() &&
            $param->isDefaultValueConstant();
    }

    public function generate(\ReflectionParameter $param, CodegenMethod $method): void {
        /**
         * HHVM seems to treat default values for bool
         * parameters like constants, php does not.
         * @see https://github.com/usox/hackmock/issues/3.
         * @see https://github.com/usox/hackmock/pull/4.
         */
        $method->addParameterf(
            '%s $%s = %s',
            (string) $param->getType(),
            $param->getName(),
            $param->getDefaultValueText()
        );
    }
}
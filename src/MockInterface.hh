<?hh // partial
namespace Usox\HackMock;

interface MockInterface {

	public function expects(string $method_name): ExpectationInterface;

	public function build();
}

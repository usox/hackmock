<?hh // strict
namespace Usox\HackMock;

use type Facebook\HackCodegen\HackCodegenFactory;
use type Facebook\HackCodegen\HackCodegenConfig;
use namespace HH\Lib\{Str};

final class Mock<TC> implements MockInterface {

	public function __construct(private classname<TC> $interface_name): void {
	}

	public function build(): TC {
		$rfl = new \ReflectionClass($this->interface_name);

		$mock_name = Str\format(
			'%s_Implementation%s',
			$rfl->getShortName(),
			\spl_object_hash($this)
		);

		/* HH_FIXME[2049] **/
		/* HH_FIXME[4107] **/
		eval(
			(new Mock\MockBuilder($rfl))
			->setName($mock_name)
			->get()
			->render()
		);

		/* HH_FIXME[4110] **/
		/* HH_FIXME[4026] **/
		return new $mock_name();
	}
}

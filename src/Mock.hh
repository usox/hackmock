<?hh // strict
namespace Usox\HackMock;

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

		$name = (string)\microtime(true);

		\file_put_contents(
			'/tmp/aggi'.$name.'.hack',
			(new Mock\MockBuilder($rfl))
			->setName($mock_name)
			->get()
			->render()
		);

		require_once('/tmp/aggi'.$name.'.hack');

		\unlink('/tmp/aggi'.$name.'.hack');

		/* HH_FIXME[4110] **/
		/* HH_FIXME[4026] **/
		return new $mock_name();
	}
}

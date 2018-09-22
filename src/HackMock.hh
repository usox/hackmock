<?hh //strict

namespace Usox\HackMock;

use type Facebook\HackTest\HackTest;

abstract class HackMock extends HackTest {

	<<__Override>>
	public async function afterEachTestAsync(): Awaitable<void> {
		\Usox\HackMock\tear_down();
	}
}

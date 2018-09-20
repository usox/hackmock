<?hh //strict

namespace Usox\HackMock;

use Facebook\HackTest\HackTest;

abstract class HackMock extends HackTest {

	public async function afterEachTestAsync(): Awaitable<void> {
		\Usox\HackMock\tear_down();
	}
}
#include "Main.h"
#include <vector>

int main()
{
	GameBoy::Cartridge* cartridge = new GameBoy::Cartridge("./Pokemon - Blue Version (USA, Europe) (SGB Enhanced).gb");

	system("pause");
	return 0;
}

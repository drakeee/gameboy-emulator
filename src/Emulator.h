#pragma once

#include <Main.h>

namespace GameBoy
{
	class Emulator
	{
	public:
		Emulator();
		~Emulator();

		CPU* cpu = nullptr;
		Memory* memory = nullptr;
		Cartridge* cartridge = nullptr;
		Graphics* graphics = nullptr;	
	};
}
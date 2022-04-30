#pragma once

#include <Main.h>

namespace GameBoy
{
	class CPU;
	class Memory;
	class Cartridge;
	class Graphics;

	class Emulator
	{
	public:
		Emulator(const char* filePath);
		Emulator(const std::string& filePath);
		~Emulator();

		CPU* cpu = nullptr;
		Memory* memory = nullptr;
		Cartridge* cartridge = nullptr;
		Graphics* graphics = nullptr;	
	};
}
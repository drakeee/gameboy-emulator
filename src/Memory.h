#pragma once

#include <Main.h>

namespace GameBoy
{
	class Memory
	{
	public:
		Memory(Cartridge* cartridge);
		Memory(uint8_t data[0xFFFF]);
		~Memory();

		void Write(uint16_t address, uint8_t value);
		uint8_t Read(uint16_t address);
	private:
		uint8_t memory[0xFFFF];
	};
}
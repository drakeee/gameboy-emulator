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

		inline uint8_t* GetMemory() { return this->m_Memory; }
		inline uint8_t* GetFullROM() { return this->m_Rom; }
		inline uint32_t GetROMSize() { return this->m_RomSize; }
	private:
		uint8_t m_Memory[0xFFFF + 1];
		uint8_t* m_Rom = nullptr;
		uint32_t m_RomSize = 0;
	};
}
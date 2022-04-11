#pragma once

#include <Main.h>

namespace GameBoy
{
	class Memory
	{
	public:
		Memory();
		~Memory();

		void Write();
		void Read();
	private:
		uint8_t memory[0xFFFF];
	};
}
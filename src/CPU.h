#pragma once

#include <Main.h>

namespace GameBoy
{
	class CPU
	{
	public:
		enum RegisterFlag
		{
			ZERO = 0,
			SUBTRACTION,
			HALF_CARRY,
			CARRY
		};

		struct RegisterPair //yes, I know that I can achieve the same result using union and struct as well
		{
		public:
			RegisterPair()
			{

			}

			RegisterPair(uint16_t value)
			{
				this->SetValue(value);
			}

			RegisterPair(int value)
			{
				this->SetValue((uint16_t)value);
			}

			uint8_t lsb = 0;
			uint8_t msb = 0;

			uint8_t GetLSB()
			{
				return this->lsb;
			}

			uint8_t GetMSB()
			{
				return this->msb;
			}

			void SetValue(uint16_t value)
			{
				this->msb = static_cast<uint8_t>((value & 0xFF00) >> 8);
				this->lsb = static_cast<uint8_t>(value & 0x00FF);
			}

			uint16_t GetValue()
			{
				return ((uint16_t)this->msb << 8) | this->lsb;
			}

			RegisterPair& operator=(const RegisterPair& rhs)
			{
				lsb = rhs.lsb;
				msb = rhs.msb;
				return *this;
			}

			RegisterPair& operator=(const uint16_t value)
			{
				SetValue(value);
				return *this;
			}

			RegisterPair& operator=(const int value)
			{
				SetValue((uint16_t)value);
				return *this;
			}
		};

		CPU(Memory* memory);
		~CPU();

	private:
		void ProcessInstruction();

		Memory* memory = nullptr;

		//Register pairs
		//Note: Values are set to Bootstrap default values (https://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM)
		RegisterPair AF = 0x01B0;
		RegisterPair BC = 0x0013;
		RegisterPair DE = 0x00D8;
		RegisterPair HL = 0x014D;

		//Stack Pointer
		uint16_t SP = 0xFFFE;

		//Program Counter
		uint16_t PC = 0x0100;
	};
}
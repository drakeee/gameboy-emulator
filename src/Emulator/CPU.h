#pragma once

#include <Main.h>
#include <ostream>
#include <string>

#define SET_BIT(a,b) (a|=b)
#define CLEAR_BIT(a,b) (a&=~(b))
#define TOGGLE_BIT(a,b) (a^=b)
#define CHECK_BIT(a,b) ((a&(b))==(b))

namespace GameBoy
{
	class CPU
	{
	public:
		enum RegisterFlag : uint8_t
		{
			CARRY		= (1 << 4), //CY
			HALF_CARRY	= (1 << 5), //H
			SUBTRACTION	= (1 << 6), //N
			ZERO		= (1 << 7) //Z
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

			RegisterPair operator+(RegisterPair& obj) {
				RegisterPair res;
				res = (GetValue() + obj.GetValue());
				return res;
			}

			RegisterPair& operator+=(RegisterPair& rhs)
			{
				SetValue(GetValue() + rhs.GetValue());
				return *this;
			}

			RegisterPair& operator+=(uint16_t value)
			{
				SetValue(GetValue() + value);
				return *this;
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

			std::ostream& operator<<(std::ostream& os)
			{
				return os << std::to_string(this->GetValue()) << std::endl;
			}
		};

		CPU(Memory* memory);
		~CPU();

		void ProcessInstruction();

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

	private:
		void ProcessCBInstruction();

		void UnimplementedInstruction(const char* msg = "");

		Memory* memory = nullptr;

	private:
		uint8_t read_byte();
		uint16_t read_word();


		//Source: https://stackoverflow.com/a/57822729
		inline bool half_carry_add(uint8_t first_num, uint8_t second_num) { return (((first_num & 0x0F) + (second_num & 0x0F)) & 0x10) == 0x10; }
		inline bool half_carry_add(uint16_t first_num, uint16_t second_num) { return (((first_num & 0x00FF) + (second_num & 0x00FF)) & 0x0100) == 0x0100; }

		inline bool half_carry_sub(uint8_t first_num, uint8_t second_num) { return (int)(first_num & 0x0F) - (int)(second_num & 0x0F) < 0; }
		inline bool half_carry_sub(uint16_t first_num, uint16_t second_num) { return (int)(first_num & 0x00FF) - (int)(second_num & 0x00FF) < 0; }

		inline bool carry(uint8_t first_num, uint8_t second_num)
		{
			uint16_t fullResult = first_num + second_num;
			return fullResult > 0xFF;
		}

		inline bool carry(uint16_t first_num, uint16_t second_num)
		{
			uint32_t fullResult = first_num + second_num;
			return fullResult > 0xFFFF;
		}

		inline void set_flag_value(RegisterFlag flag, bool value)
		{
			if (value)	SET_BIT(this->AF.lsb, flag);
			else		CLEAR_BIT(this->AF.lsb, flag);
		}

		inline bool get_flag_value(RegisterFlag flag)
		{
			return CHECK_BIT(this->AF.lsb, flag);
		}

		void opcode_ld(RegisterPair& pair); //read uint16_t into register pair
		void opcode_ld(uint8_t& value); //read uint8_t into register pair's
		void opcode_ld(uint8_t& value, RegisterPair& address);
		void opcode_ld(RegisterPair& pair, uint8_t value); //write value in the memory location specified by register pair
		void opcode_ld(uint16_t address, uint16_t value);

		void opcode_inc(RegisterPair& pair);
		void opcode_inc(uint8_t& value);

		void opcode_dec(RegisterPair& pair);
		void opcode_dec(uint8_t& value);

		void opcode_rlc(uint8_t& value); //rotate left
		void opcode_rrc(uint8_t& value); //rotate right
		void opcode_rl(uint8_t& value);
		void opcode_rr(uint8_t& value);

		void opcode_add(RegisterPair& one, RegisterPair& two);
	};
}
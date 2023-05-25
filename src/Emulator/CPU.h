#pragma once

#include <Main.h>
#include <ostream>
#include <string>
#include <stack>
#include <array>

#define BIT(a) (1<<(a))
#define SET_BIT(a,b) ((a)|=(b))
#define CLEAR_BIT(a,b) ((a)&=~(b))
#define TOGGLE_BIT(a,b) ((a)^=(b))
#define CHECK_BIT(a,b) (((a)&(b))==(b))
//#define CHECK_BIT(a,b)( ((a)&(1<<(b)))==(1<<(b)))

namespace GameBoy
{
	class Emulator;
	class CPU
	{
	public:

		static const std::array<uint8_t, 256> boot_rom;
		static const std::array<std::string, 256> opcode_names;
		static const std::array<std::string, 256> opcode_cb_names;
		static const std::array<uint8_t, 256> opcode_cycles;
		static const std::array<uint8_t, 256> opcode_lengths;

		enum RegisterFlag : uint8_t
		{
			CARRY		= BIT(4), //CY
			HALF_CARRY	= BIT(5), //H
			SUBTRACTION	= BIT(6), //N
			ZERO		= BIT(7) //Z
		};

		enum class Condition : uint8_t
		{
			NONE = 0,

			CARRY,
			HALF_CARRY,
			SUBTRACTION,
			ZERO,

			NOT_CARRY,
			NOT_HALF_CARRY,
			NOT_SUBTRACTION,
			NOT_ZERO
		};

		struct RegisterPair //yes, I know that I can achieve the same result using union and struct as well
		{
		public:
			RegisterPair()
			{

			}

			RegisterPair(uint16_t value)
			{
				if (value == '\n')
					value = 0;

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

			RegisterPair& operator++()
			{
				SetValue(GetValue() + 1);
				return *this;
			}

			RegisterPair& operator--()
			{
				SetValue(GetValue() - 1);
				return *this;
			}

			RegisterPair operator++(int)
			{
				RegisterPair temp = *this;
				++*this;
				return temp;
			}

			RegisterPair operator--(int)
			{
				RegisterPair temp = *this;
				--*this;
				return temp;
			}

			RegisterPair& operator+(RegisterPair& obj) {
				*this = (GetValue() + obj.GetValue());
				return *this;
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

			inline operator uint16_t() { return GetValue(); }

			std::ostream& operator<<(std::ostream& os)
			{
				return os << std::to_string(this->GetValue()) << std::endl;
			}
		};

		CPU(Emulator*);
		~CPU();

		void HandleInterrupts();
		void ProcessInstruction();

		//Register pairs
		//Note: Values are set to Bootstrap default values (https://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM)
		RegisterPair AF = 0x01B0;
		RegisterPair BC = 0x0013;
		RegisterPair DE = 0x00D8;
		RegisterPair HL = 0x014D;

		FILE* debugFile = nullptr;

		//Stack Pointer
		uint16_t SP = 0xFFFE;

		//Program Counter
		uint16_t PC = 0x0100;

		bool debug = false;
		bool interrupts_enabled = false;

	private:
		void ProcessCBInstruction();

		void UnimplementedInstruction(const char* msg = "");

		Emulator* emulator = nullptr;

	public:
		uint8_t read_byte();
		uint16_t read_word();


		//Source: https://gist.github.com/meganesu/9e228b6b587decc783aa9be34ae27841
		inline bool half_carry_add(uint8_t first_num, uint8_t second_num, bool carry = false) { return ((first_num & 0x0F) + (second_num & 0x0F) + ((uint8_t)carry & 0x0F)) & 0x10; }
		inline bool half_carry_add(uint16_t first_num, uint16_t second_num, bool carry = false) { return (((first_num & 0xFFF) + (second_num & 0xFFF) + ((uint16_t)carry & 0xFFF)) & 0x1000) == 0x1000; }

		/*inline bool half_carry_sub(uint8_t first_num, uint8_t second_num)
		{
			std::bitset<8> x(first_num - second_num);
			return x.test(3);
		}

		inline bool half_carry_sub(uint16_t first_num, uint16_t second_num)
		{
			std::bitset<8> x(first_num - second_num);
			return x.test(3);
		}*/

		inline bool half_carry_sub(uint8_t first_num, uint8_t second_num, bool carry = false)
		{
			return ((first_num & 0xf) - (second_num & 0xf) - ((int)carry & 0xf)) & 0x10;
		}

		inline bool carry(uint8_t first_num, uint32_t second_num)
		{
			uint32_t fullResult = first_num + second_num;
			return fullResult > 0xFF;
		}

		inline bool carry(uint16_t first_num, uint32_t second_num)
		{
			uint32_t fullResult = first_num + second_num;
			return fullResult > 0xFFFF;
		}

		inline bool carry_sub(uint8_t first_num, uint32_t second_num)
		{
			int fullResult = first_num - second_num;
			return fullResult < 0;
		}

		inline bool carry_sub(uint16_t first_num, uint32_t second_num)
		{
			int fullResult = first_num - second_num;
			return fullResult < 0;
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

		void push_sp(RegisterPair& pair);
		void push_sp(uint16_t value);
		void pop_sp(RegisterPair& pair);
		uint16_t pop_sp();

		void opcode_di();
		void opcode_ei();

		void opcode_daa(); //Source: https://github.com/jgilchrist/gbemu/blob/6da040537acc003c9374e66af5de801b4623711c/src/cpu/opcodes.cc#L205

		void opcode_ld(RegisterPair& pair); //read uint16_t into register pair
		void opcode_ld(uint8_t& value); //read uint8_t into register pair's
		void opcode_ld(uint16_t& value);
		void opcode_ld(uint8_t& value, RegisterPair& address);
		void opcode_ld(RegisterPair& pair, uint8_t value); //write value in the memory location specified by register pair
		void opcode_ld(uint16_t address, uint16_t value);
		void opcode_ld(uint16_t address, uint8_t value);
		void opcode_ld(uint8_t& one, uint8_t two);
		void opcode_ld_sp_into_hl();

		void opcode_ld_memory(uint8_t address, uint8_t value);
		void opcode_ld_memory(RegisterPair& pair);
		void opcode_ld_memory(RegisterPair& pair, RegisterPair& value);

		void opcode_ldc_a_into_a8();
		void opcode_ldc_a_into_c();
		void opcode_ldc_a8_into_a();
		void opcode_ldc_c_into_a();

		void opcode_inc(RegisterPair& pair);
		void opcode_inc(uint8_t& value);
		void opcode_inc(uint16_t& value);
		void opcode_inc_memory(RegisterPair& pair);

		void opcode_dec(RegisterPair& pair);
		void opcode_dec(uint8_t& value);
		void opcode_dec(uint16_t& value);
		void opcode_dec_memory(RegisterPair& pair);

		void opcode_rlc(uint8_t& value); //rotate left
		void opcode_rrc(uint8_t& value); //rotate right
		void opcode_rl(uint8_t& value);
		void opcode_rr(uint8_t& value);

		void opcode_add(RegisterPair& one, RegisterPair& two);
		void opcode_add(RegisterPair& one, uint16_t two);
		void opcode_add(uint8_t& one, uint8_t two);
		void opcode_add(uint8_t& one, RegisterPair& address);
		void opcode_add_sp();

		void opcode_adc(uint8_t& one, uint8_t two);
		void opcode_adc(uint8_t& one, RegisterPair& address);

		void opcode_sub(uint8_t one);
		void opcode_sub(RegisterPair& address);
		void opcode_sbc(uint8_t& one, uint8_t two);
		void opcode_sbc(uint8_t& one, RegisterPair& address);

		void opcode_and(uint8_t one);
		void opcode_and(RegisterPair& address);

		void opcode_xor(uint8_t one);
		void opcode_xor(RegisterPair& address);

		void opcode_or(uint8_t one);
		void opcode_or(RegisterPair& address);

		void opcode_cp(uint8_t one);
		void opcode_cp(RegisterPair& address);

		void opcode_ret(Condition condition = Condition::NONE);
		void opcode_call(Condition condition = Condition::NONE);
		void opcode_jp(Condition condition = Condition::NONE);
		void opcode_jp_hl();

		void opcode_rst(uint8_t value);

		void opcode_pop(RegisterPair& pair);
		void opcode_push(RegisterPair& pair);

		bool if_condition(Condition condition);
		void opcode_jr(Condition condition = Condition::NONE);

		void opcode_cpl(void);
		void opcode_scf(void);
		void opcode_ccf(void);

		//CB instructions
		void opcode_cb_rlc(uint8_t& value);
		void opcode_cb_rlc(RegisterPair& address);

		void opcode_cb_rrc(uint8_t& value);
		void opcode_cb_rrc(RegisterPair& address);

		void opcode_cb_rl(uint8_t& value);
		void opcode_cb_rl(RegisterPair& address);

		void opcode_cb_rr(uint8_t& value);
		void opcode_cb_rr(RegisterPair& address);

		void opcode_cb_sla(uint8_t& value);
		void opcode_cb_sla(RegisterPair& address);

		void opcode_cb_sra(uint8_t& value);
		void opcode_cb_sra(RegisterPair& address);

		void opcode_cb_swap(uint8_t& value);
		void opcode_cb_swap(RegisterPair& address);

		void opcode_cb_srl(uint8_t& value);
		void opcode_cb_srl(RegisterPair& address);

		void opcode_cb_bit(uint8_t bit, uint8_t& value);
		void opcode_cb_bit(uint8_t bit, RegisterPair& address);

		void opcode_cb_res(uint8_t bit, uint8_t& value);
		void opcode_cb_res(uint8_t bit, RegisterPair& address);

		void opcode_cb_set(uint8_t bit, uint8_t& value);
		void opcode_cb_set(uint8_t bit, RegisterPair& address);
	};
}
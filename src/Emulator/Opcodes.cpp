#include <Emulator/CPU.h>

using namespace GameBoy;

uint8_t CPU::read_byte()
{
	uint8_t byte = this->memory->Read(this->PC);
	this->PC++;

	return byte;
}

uint16_t CPU::read_word()
{
	uint16_t lsb = this->memory->Read(this->PC);
	uint16_t msb = this->memory->Read(this->PC + 1);

	this->PC += 2;

	return (((uint16_t)msb << 8) | lsb);
}

void CPU::opcode_ld(RegisterPair& pair)
{
	pair = this->read_word();
}

void CPU::opcode_ld(uint8_t& value)
{
	value = this->read_byte();
}

void CPU::opcode_ld(uint8_t& value, RegisterPair& address)
{
	value = this->memory->Read(address.GetValue());
}

void CPU::opcode_ld(RegisterPair& pair, uint8_t value)
{
	this->memory->Write(pair.GetValue(), value);
}

void CPU::opcode_ld(uint16_t address, uint16_t value)
{
	uint8_t lower = (value & 0x00FF);
	uint8_t higher = ((value >> 8) & 0xFF);

	this->memory->Write(address, lower);
	this->memory->Write(address + 1, higher);
}

void CPU::opcode_inc(RegisterPair& pair)
{
	pair = (pair.GetValue() + 1);
}

void CPU::opcode_inc(uint8_t& value)
{
	value += 1;

	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::ZERO, (value == 0));
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_add(uint8_t(value - 1), uint8_t(1)));
}

void CPU::opcode_dec(RegisterPair& pair)
{
	pair = (pair.GetValue() - 1);
}

void CPU::opcode_dec(uint8_t& value)
{
	value -= 1;

	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::ZERO, (value == 0));
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_sub(uint8_t(value + 1), uint8_t(1)));
}

void CPU::opcode_rlc(uint8_t& value)
{
	bool carry = ((value >> 7) & 0x1);
	uint8_t result = ((value << 1) | carry);
	value = result;

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::CARRY, carry);
}

void CPU::opcode_rrc(uint8_t& value)
{
	bool carry = CHECK_BIT(value, (1 << 0));
	uint8_t result = ((value >> 1) | carry);
	value = result;

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::CARRY, carry);
}

void CPU::opcode_rl(uint8_t& value)
{
	bool carry = this->get_flag_value(RegisterFlag::CARRY);
	uint8_t result = ((value << 1) | carry);

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::CARRY, carry);
}

void CPU::opcode_rr(uint8_t& value)
{

}

void CPU::opcode_add(RegisterPair& one, RegisterPair& two)
{
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_add(one.GetValue(), two.GetValue()));
	this->set_flag_value(RegisterFlag::CARRY, this->carry(one.GetValue(), two.GetValue()));

	one += two;
}
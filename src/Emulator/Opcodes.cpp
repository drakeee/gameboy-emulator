#include <Emulator/CPU.h>

#include <bitset>

using namespace GameBoy;

uint8_t CPU::read_byte()
{
	uint8_t byte = this->emulator->memory->Read(this->PC);
	this->PC++;

	return byte;
}

uint16_t CPU::read_word()
{
	uint16_t lsb = this->emulator->memory->Read(this->PC);
	uint16_t msb = this->emulator->memory->Read(this->PC + 1);

	this->PC += 2;

	RegisterPair tmp;
	tmp.lsb = lsb;
	tmp.msb = msb;

	return tmp.GetValue();
}

void CPU::push_sp(RegisterPair& pair)
{
	this->emulator->memory->Write(this->SP - 1, pair.msb);
	this->emulator->memory->Write(this->SP - 2, pair.lsb);
	this->SP -= 2;
}

void CPU::push_sp(uint16_t value)
{
	RegisterPair pair;
	pair.SetValue(value);

	this->push_sp(pair);
}

void CPU::pop_sp(RegisterPair& pair)
{
	pair.lsb = this->emulator->memory->Read(this->SP);
	pair.msb = this->emulator->memory->Read(this->SP + 1);
	this->SP += 2;
}

uint16_t CPU::pop_sp()
{
	RegisterPair pair;
	pop_sp(pair);

	return pair.GetValue();
}

void CPU::opcode_di()
{
	//debug = true;
	this->interrupts_enabled = false;
}

void CPU::opcode_ei()
{
	printf("enable interrupts\n");
	this->interrupts_enabled = true;
}

void CPU::opcode_daa()
{
	//Source: https://gbdev.gg8.se/wiki/articles/DAA

	if (this->get_flag_value(RegisterFlag::SUBTRACTION))
	{
		if (this->get_flag_value(RegisterFlag::CARRY)) { this->AF.msb -= 0x60; }
		if (this->get_flag_value(RegisterFlag::HALF_CARRY)) { this->AF.msb -= 0x06; }
	}
	else
	{
		if (this->get_flag_value(RegisterFlag::CARRY) || (this->AF.msb & 0xFF) > 0x99) { this->AF.msb += 0x60; this->set_flag_value(RegisterFlag::CARRY, true); }
		if (this->get_flag_value(RegisterFlag::HALF_CARRY) || (this->AF.msb & 0x0F) > 0x09) { this->AF.msb += 0x06; }
	}

	this->set_flag_value(RegisterFlag::ZERO, this->AF.msb == 0);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);

	/*uint8_t reg = this->AF.msb;

	uint16_t correction = this->get_flag_value(RegisterFlag::CARRY)
		? 0x60
		: 0x00;

	if (this->get_flag_value(RegisterFlag::CARRY) || (!this->get_flag_value(RegisterFlag::SUBTRACTION) && ((reg & 0x0F) > 9))) {
		correction |= 0x06;
	}

	if (this->get_flag_value(RegisterFlag::CARRY) || (!this->get_flag_value(RegisterFlag::SUBTRACTION) && (reg > 0x99))) {
		correction |= 0x60;
	}

	if (this->get_flag_value(RegisterFlag::SUBTRACTION)) {
		reg = static_cast<uint8_t>(reg - correction);
	}
	else {
		reg = static_cast<uint8_t>(reg + correction);
	}

	if (((correction << 2) & 0x100) != 0) {
		this->set_flag_value(RegisterFlag::CARRY, true);
	}

	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::ZERO, reg == 0);

	this->AF.msb = reg;*/
	
	/*
	int tmp = this->AF.msb;

	bool FLAG_N = this->get_flag_value(RegisterFlag::SUBTRACTION);
	bool FLAG_H = this->get_flag_value(RegisterFlag::HALF_CARRY);
	bool FLAG_C = this->get_flag_value(RegisterFlag::CARRY);
	bool FLAG_Z = this->get_flag_value(RegisterFlag::ZERO);

	if (!FLAG_N) {
		if (FLAG_H || (tmp & 0x0F) > 9)
			tmp += 6;
		if (FLAG_C || tmp > 0x9F)
			tmp += 0x60;
	}
	else {
		if (FLAG_H) {
			tmp -= 6;
			if (!(FLAG_C))
				tmp &= 0xFF;
		}
		if (FLAG_C)
			tmp -= 0x60;
	}

	this->AF.lsb &= ~(FLAG_H | FLAG_Z);
	if (tmp & 0x100)
		this->AF.lsb |= FLAG_C;
	this->AF.lsb = tmp & 0xFF;
	if (!this->AF.msb)
		this->AF.lsb |= FLAG_Z;

	this->AF.lsb &= 0xF0;*/
}

void CPU::opcode_ld(RegisterPair& pair)
{
	pair = this->read_word();
}

void CPU::opcode_ld(uint8_t& value)
{
	value = this->read_byte();
}

void CPU::opcode_ld(uint16_t& value)
{
	value = this->read_word();
}

void CPU::opcode_ld(uint8_t& value, RegisterPair& address)
{
	value = this->emulator->memory->Read(address.GetValue());
}

void CPU::opcode_ld(RegisterPair& pair, uint8_t value)
{
	this->emulator->memory->Write(pair.GetValue(), value);
}

void CPU::opcode_ld(uint16_t address, uint16_t value)
{
	RegisterPair tmp = value;

	this->emulator->memory->Write(address, tmp.lsb);
	this->emulator->memory->Write(address + 1, tmp.msb);
}

void CPU::opcode_ld(uint16_t address, uint8_t value)
{
	this->emulator->memory->Write(address, value);
}

void CPU::opcode_ld(uint8_t& one, uint8_t two)
{
	one = two;
}

void CPU::opcode_ld_sp_into_hl()
{
	int8_t value = this->read_byte();
	int result = static_cast<int>(this->SP + value);

	this->set_flag_value(RegisterFlag::HALF_CARRY, ((this->SP ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10);
	this->set_flag_value(RegisterFlag::CARRY, ((this->SP ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100);

	this->HL = this->SP + value;

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
}

void CPU::opcode_ld_memory(uint8_t address, uint8_t value)
{
	this->emulator->memory->Write(address, value);
}

void CPU::opcode_ld_memory(RegisterPair& pair)
{
	uint8_t value = this->read_byte();
	this->emulator->memory->Write(pair.GetValue(), value);
}

void CPU::opcode_ld_memory(RegisterPair& pair, RegisterPair& value)
{
	this->emulator->memory->Write(pair.GetValue(), value.lsb);
	this->emulator->memory->Write(pair.GetValue() + 1, value.lsb);
}

void CPU::opcode_ldc_a_into_a8()
{
	int8_t address = this->read_byte();

	RegisterPair tmp;
	tmp.lsb = address;
	tmp.msb = 0xFF;

	this->emulator->memory->Write(tmp.GetValue(), this->AF.msb);
}

void CPU::opcode_ldc_a_into_c()
{
	RegisterPair tmp;
	tmp.lsb = this->BC.lsb;
	tmp.msb = 0xFF;

	this->emulator->memory->Write(tmp.GetValue(), this->AF.msb);
}

void CPU::opcode_ldc_a8_into_a()
{
	uint8_t value = this->read_byte();

	this->AF.msb = this->emulator->memory->Read(0xFF00 + value);
}

void CPU::opcode_ldc_c_into_a()
{
	RegisterPair tmp;
	tmp.lsb = this->BC.lsb;
	tmp.msb = 0xFF;

	this->AF.msb = this->emulator->memory->Read(tmp.GetValue());
}

void CPU::opcode_inc(RegisterPair& pair)
{
	pair = (pair.GetValue() + 1);
}

void CPU::opcode_inc(uint8_t& value)
{
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_add(value, 1));
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);

	value += 1;
	this->set_flag_value(RegisterFlag::ZERO, (value == 0));
}

void CPU::opcode_inc(uint16_t& value)
{
	value += 1;
}

void CPU::opcode_inc_memory(RegisterPair& pair)
{
	uint8_t value = this->emulator->memory->Read(pair.GetValue());
	opcode_inc(value);

	this->emulator->memory->Write(pair.GetValue(), value);
}

void CPU::opcode_dec(RegisterPair& pair)
{
	pair = (pair.GetValue() - 1);
}

void CPU::opcode_dec(uint8_t& value)
{
	this->set_flag_value(RegisterFlag::SUBTRACTION, true);
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_sub(value, 1));

	value -= 1;
	this->set_flag_value(RegisterFlag::ZERO, (value == 0));
}

void CPU::opcode_dec(uint16_t& value)
{
	value -= 1;
}

void CPU::opcode_dec_memory(RegisterPair& pair)
{
	uint8_t value = this->emulator->memory->Read(pair.GetValue());
	opcode_dec(value);

	this->emulator->memory->Write(pair.GetValue(), value);
}

void CPU::opcode_rlc(uint8_t& value)
{
	std::bitset<8> tmp = value;
	bool carry = tmp.test(7);

	tmp = (value << 1);
	tmp.set(0, carry);
	value = (uint8_t)tmp.to_ulong();

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::CARRY, carry);
}

void CPU::opcode_rrc(uint8_t& value)
{
	std::bitset<8> tmp = value;
	bool carry = tmp.test(0);

	tmp = value >> 1;
	tmp.set(7, carry);

	value = (uint8_t)tmp.to_ulong();

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::CARRY, carry);
}

void CPU::opcode_rl(uint8_t& value)
{
	bool oldCarry = this->get_flag_value(RegisterFlag::CARRY);
	
	std::bitset<8> tmp = value;
	bool newCarry = tmp.test(7);

	tmp = value << 1;
	tmp.set(0, oldCarry);

	value = (uint8_t)tmp.to_ulong();

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::CARRY, newCarry);
}

void CPU::opcode_rr(uint8_t& value)
{
	bool oldCarry = this->get_flag_value(RegisterFlag::CARRY);
	std::bitset<8> tmp = value;
	bool newCarry = tmp.test(0);

	tmp = value >> 1;
	tmp.set(7, oldCarry);

	value = (uint8_t)tmp.to_ulong();

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::CARRY, newCarry);
}

void CPU::opcode_add(RegisterPair& one, RegisterPair& two)
{
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_add(one.GetValue(), two.GetValue()));
	this->set_flag_value(RegisterFlag::CARRY, this->carry(one.GetValue(), two.GetValue()));

	one += two;
}

void CPU::opcode_add(RegisterPair& one, uint16_t two)
{
	opcode_add(one, static_cast<RegisterPair&>(RegisterPair(two)));
}

void CPU::opcode_add(uint8_t& one, uint8_t two)
{
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_add(one, two));
	this->set_flag_value(RegisterFlag::CARRY, this->carry(one, two));

	one += two;
	this->set_flag_value(RegisterFlag::ZERO, one == 0);
}

void CPU::opcode_add(uint8_t& one, RegisterPair& address)
{
	this->opcode_add(one, this->emulator->memory->Read(address.GetValue()));
}

void CPU::opcode_add_sp()
{
	//Source: https://github.com/jgilchrist/gbemu/blob/6da040537acc003c9374e66af5de801b4623711c/src/cpu/opcodes.cc#L87

	int8_t value = this->read_byte();
	int result = static_cast<int>(this->SP + value);

	this->set_flag_value(RegisterFlag::HALF_CARRY, ((this->SP ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10);
	this->set_flag_value(RegisterFlag::CARRY, ((this->SP ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100);

	this->SP += value;

	this->set_flag_value(RegisterFlag::ZERO, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
}

void CPU::opcode_adc(uint8_t& one, uint8_t two)
{
	bool carry = this->get_flag_value(RegisterFlag::CARRY);

	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_add(one, two, carry));
	this->set_flag_value(RegisterFlag::CARRY, this->carry(one, two + carry));

	one = (one + two + (uint8_t)carry);
	this->set_flag_value(RegisterFlag::ZERO, one == 0);
}

void CPU::opcode_adc(uint8_t& one, RegisterPair& address)
{
	this->opcode_adc(one, this->emulator->memory->Read(address.GetValue()));
}

void CPU::opcode_sub(uint8_t value)
{
	this->set_flag_value(RegisterFlag::SUBTRACTION, true);
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_sub(this->AF.msb, value));
	this->set_flag_value(RegisterFlag::CARRY, this->carry_sub(this->AF.msb, value));

	this->AF.msb -= value;
	this->set_flag_value(RegisterFlag::ZERO, this->AF.msb == 0);
}

void CPU::opcode_sub(RegisterPair& address)
{
	this->opcode_sub(this->emulator->memory->Read(address.GetValue()));
}

void CPU::opcode_sbc(uint8_t& one, uint8_t value)
{
	bool carry = this->get_flag_value(RegisterFlag::CARRY);

	this->set_flag_value(RegisterFlag::SUBTRACTION, true);
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_sub(one, value, carry));
	this->set_flag_value(RegisterFlag::CARRY, this->carry_sub(one, value + carry));

	one = (one - value - carry);
	this->set_flag_value(RegisterFlag::ZERO, one == 0);
}

void CPU::opcode_sbc(uint8_t& one, RegisterPair& address)
{
	this->opcode_sbc(one, this->emulator->memory->Read(address.GetValue()));
}

void CPU::opcode_and(uint8_t one)
{
	this->AF.msb = this->AF.msb & one;

	this->set_flag_value(RegisterFlag::ZERO, this->AF.msb == 0);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, true);
	this->set_flag_value(RegisterFlag::CARRY, false);
}

void CPU::opcode_and(RegisterPair& address)
{
	this->opcode_and(this->emulator->memory->Read(address.GetValue()));
}

void CPU::opcode_xor(uint8_t one)
{
	this->AF.msb = this->AF.msb ^ one;

	this->set_flag_value(RegisterFlag::ZERO, this->AF.msb == 0);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::CARRY, false);
}

void CPU::opcode_xor(RegisterPair& address)
{
	this->opcode_xor(this->emulator->memory->Read(address.GetValue()));
}

void CPU::opcode_or(uint8_t one)
{
	this->AF.msb = this->AF.msb | one;

	this->set_flag_value(RegisterFlag::ZERO, this->AF.msb == 0);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::CARRY, false);
}

void CPU::opcode_or(RegisterPair& address)
{
	this->opcode_or(this->emulator->memory->Read(address.GetValue()));
}

void CPU::opcode_cp(uint8_t one)
{
	this->set_flag_value(RegisterFlag::ZERO, (this->AF.msb - one) == 0);
	this->set_flag_value(RegisterFlag::SUBTRACTION, true);
	this->set_flag_value(RegisterFlag::HALF_CARRY, this->half_carry_sub(this->AF.msb, one));
	this->set_flag_value(RegisterFlag::CARRY, this->carry_sub(this->AF.msb, one));
}

void CPU::opcode_cp(RegisterPair& address)
{
	this->opcode_cp(this->emulator->memory->Read(address.GetValue()));
}

void CPU::opcode_ret(Condition condition)
{
	if (if_condition(condition))
	{
		this->PC = this->pop_sp();
	}
}

void CPU::opcode_call(Condition condition)
{
	uint16_t address = this->read_word();
	if (if_condition(condition))
	{
		this->push_sp(this->PC);

		this->PC = address;
	}
}

void CPU::opcode_jp(Condition condition)
{
	uint16_t address = this->read_word();
	if(if_condition(condition))
	{
		this->PC = address;
	}
}

void CPU::opcode_jp_hl()
{
	uint16_t address = this->HL;
	this->PC = address;
}

void CPU::opcode_rst(uint8_t value)
{
	this->push_sp(this->PC);

	RegisterPair tmp;
	tmp.msb = 0x00;
	tmp.lsb = value;

	this->PC = tmp.GetValue();
}

void CPU::opcode_pop(RegisterPair& pair)
{
	this->pop_sp(pair);
}

void CPU::opcode_push(RegisterPair& pair)
{
	this->push_sp(pair);
}

bool CPU::if_condition(Condition condition)
{
	bool result = false;

	switch (condition)
	{
	case Condition::NONE: result = true; break;
	case Condition::CARRY: result = this->get_flag_value(RegisterFlag::CARRY); break;
	case Condition::NOT_CARRY: result = !this->get_flag_value(RegisterFlag::CARRY); break;
	case Condition::ZERO: result = this->get_flag_value(RegisterFlag::ZERO); break;
	case Condition::NOT_ZERO: result = !this->get_flag_value(RegisterFlag::ZERO); break;
	}

	return result;
}

void CPU::opcode_jr(Condition condition)
{
	int8_t value = this->read_byte();

	if (if_condition(condition))
		this->PC += value;
}

void CPU::opcode_cpl(void)
{
	this->AF.msb = ~this->AF.msb;

	this->set_flag_value(RegisterFlag::SUBTRACTION, true);
	this->set_flag_value(RegisterFlag::HALF_CARRY, true);
}

void CPU::opcode_scf(void)
{
	this->set_flag_value(RegisterFlag::CARRY, true);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
}

void CPU::opcode_ccf(void)
{
	this->set_flag_value(RegisterFlag::CARRY, !this->get_flag_value(RegisterFlag::CARRY));
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
}

void CPU::opcode_cb_rlc(uint8_t& value)
{
	opcode_rlc(value);

	this->set_flag_value(RegisterFlag::ZERO, value == 0);
}

void CPU::opcode_cb_rlc(RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_rlc(tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_rrc(uint8_t& value)
{
	opcode_rrc(value);

	this->set_flag_value(RegisterFlag::ZERO, value == 0);
}

void CPU::opcode_cb_rrc(RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_rrc(tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_rl(uint8_t& value)
{
	opcode_rl(value);

	this->set_flag_value(RegisterFlag::ZERO, value == 0);
}

void CPU::opcode_cb_rl(RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_rl(tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_rr(uint8_t& value)
{
	opcode_rr(value);

	this->set_flag_value(RegisterFlag::ZERO, value == 0);
}

void CPU::opcode_cb_rr(RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_rr(tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_sla(uint8_t& value)
{
	this->set_flag_value(RegisterFlag::CARRY, CHECK_BIT(value, BIT(7)));

	value = (uint8_t)(value << 1);

	this->set_flag_value(RegisterFlag::ZERO, value == 0);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
}

void CPU::opcode_cb_sla(RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_sla(tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_sra(uint8_t& value)
{
	this->set_flag_value(RegisterFlag::CARRY, CHECK_BIT(value, BIT(0)));
	uint8_t topBit = CHECK_BIT(value, BIT(7));

	std::bitset<8> tmp = (uint8_t)(value >> 1);
	tmp.set(7, topBit);
	value = (uint8_t)tmp.to_ulong();

	this->set_flag_value(RegisterFlag::ZERO, value == 0);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
}

void CPU::opcode_cb_sra(RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_sra(tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_swap(uint8_t& value)
{
	this->set_flag_value(RegisterFlag::CARRY, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);

	value = (value << 4) | (value >> 4);
	this->set_flag_value(RegisterFlag::ZERO, value == 0);
}

void CPU::opcode_cb_swap(RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_swap(tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_srl(uint8_t& value)
{
	this->set_flag_value(RegisterFlag::CARRY, CHECK_BIT(value, BIT(0)));
	value = (value >> 1);

	this->set_flag_value(RegisterFlag::ZERO, value == 0);
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, false);
}

void CPU::opcode_cb_srl(RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_srl(tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_bit(uint8_t bit, uint8_t& value)
{
	this->set_flag_value(RegisterFlag::ZERO, !CHECK_BIT(value, BIT(bit)));
	this->set_flag_value(RegisterFlag::SUBTRACTION, false);
	this->set_flag_value(RegisterFlag::HALF_CARRY, true);
}

void CPU::opcode_cb_bit(uint8_t bit, RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_bit(bit, tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_res(uint8_t bit, uint8_t& value)
{
	std::bitset<8> tmp = value;
	tmp.set(bit, false);

	value = (uint8_t)tmp.to_ulong();
}

void CPU::opcode_cb_res(uint8_t bit, RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_res(bit, tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}

void CPU::opcode_cb_set(uint8_t bit, uint8_t& value)
{
	std::bitset<8> tmp = value;
	tmp.set(bit, true);

	value = (uint8_t)tmp.to_ulong();
}

void CPU::opcode_cb_set(uint8_t bit, RegisterPair& address)
{
	uint8_t tmp = this->emulator->memory->Read(address.GetValue());
	opcode_cb_set(bit, tmp);
	this->emulator->memory->Write(address.GetValue(), tmp);
}
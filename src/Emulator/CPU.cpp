#include <Main.h>

using namespace GameBoy;

CPU::CPU(Memory* memory) :
	memory(memory)
{
	//printf("memory: %d\n", this->memory);
}

CPU::~CPU()
{

}

void CPU::UnimplementedInstruction(const char* msg)
{
	uint8_t opcode = this->memory->Read(this->PC);
	printf("UnimplementedInstruction: OpCode: %d, %s\n", opcode, msg);
}

void CPU::ProcessInstruction()
{
	if (this->memory == nullptr)
		return;

	uint8_t opcode = this->memory->Read(this->PC);
	this->PC++;

	switch (opcode)
	{
	case 0x00: break; //NOP
	case 0x01: this->opcode_ld(this->BC); break; //LD BC, d16
	case 0x02: this->opcode_ld(this->BC, this->AF.msb); break; //LD (BC), A
	case 0x03: this->opcode_inc(this->BC); break; //INC BC
	case 0x04: this->opcode_inc(this->BC.msb); break; //INC B
	case 0x05: this->opcode_dec(this->BC.msb); break; //DEC B
	case 0x06: this->opcode_ld(this->BC.msb); break; //LD B, d8
	case 0x07: this->opcode_rlc(this->AF.msb); break; //RLCA
	case 0x08: this->opcode_ld(this->read_word(), this->SP); break; //LD (a16), SP
	case 0x09: this->opcode_add(this->HL, this->BC); break; //ADD HL, BC
	case 0x0A: this->opcode_ld(this->AF.msb, this->BC); break; //LD A, (BC)
	case 0x0B: this->opcode_dec(this->BC); break; //DEC BC
	case 0x0C: this->opcode_inc(this->BC.lsb); break; //INC C
	case 0x0D: this->opcode_dec(this->BC.lsb); break; //DEC C
	case 0x0E: this->opcode_ld(this->BC.lsb); break; //LD C, d8
	case 0x0F: this->opcode_rrc(this->AF.msb); break; //RRCA
	case 0x10: UnimplementedInstruction("STOP"); break; //STOP
	case 0x11: this->opcode_ld(this->DE); break; //LD DE, d16
	case 0x12: this->opcode_ld(this->DE, this->AF.msb); break; //LD (DE), A
	case 0x13: this->opcode_inc(this->DE); break; //INC DE
	case 0x14: this->opcode_inc(this->DE.msb); break; //INC D
	case 0x15: this->opcode_dec(this->DE.msb); break; //DEC D
	case 0x16: this->opcode_ld(this->DE.msb); break; //LD D, d8
	case 0x17: UnimplementedInstruction(); break;
	case 0x18: UnimplementedInstruction(); break;
	case 0x19: UnimplementedInstruction(); break;
	case 0x1A: UnimplementedInstruction(); break;
	case 0x1B: UnimplementedInstruction(); break;
	case 0x1C: UnimplementedInstruction(); break;
	case 0x1D: UnimplementedInstruction(); break;
	case 0x1E: UnimplementedInstruction(); break;
	case 0x1F: UnimplementedInstruction(); break;
	case 0x20: UnimplementedInstruction(); break;
	case 0x21: UnimplementedInstruction(); break;
	case 0x22: UnimplementedInstruction(); break;
	case 0x23: UnimplementedInstruction(); break;
	case 0x24: UnimplementedInstruction(); break;
	case 0x25: UnimplementedInstruction(); break;
	case 0x26: UnimplementedInstruction(); break;
	case 0x27: UnimplementedInstruction(); break;
	case 0x28: UnimplementedInstruction(); break;
	case 0x29: UnimplementedInstruction(); break;
	case 0x2A: UnimplementedInstruction(); break;
	case 0x2B: UnimplementedInstruction(); break;
	case 0x2C: UnimplementedInstruction(); break;
	case 0x2D: UnimplementedInstruction(); break;
	case 0x2E: UnimplementedInstruction(); break;
	case 0x2F: UnimplementedInstruction(); break;
	case 0x30: UnimplementedInstruction(); break;
	case 0x31: UnimplementedInstruction(); break;
	case 0x32: UnimplementedInstruction(); break;
	case 0x33: UnimplementedInstruction(); break;
	case 0x34: UnimplementedInstruction(); break;
	case 0x35: UnimplementedInstruction(); break;
	case 0x36: UnimplementedInstruction(); break;
	case 0x37: UnimplementedInstruction(); break;
	case 0x38: UnimplementedInstruction(); break;
	case 0x39: UnimplementedInstruction(); break;
	case 0x3A: UnimplementedInstruction(); break;
	case 0x3B: UnimplementedInstruction(); break;
	case 0x3C: UnimplementedInstruction(); break;
	case 0x3D: UnimplementedInstruction(); break;
	case 0x3E: UnimplementedInstruction(); break;
	case 0x3F: UnimplementedInstruction(); break;
	case 0x40: UnimplementedInstruction(); break;
	case 0x41: UnimplementedInstruction(); break;
	case 0x42: UnimplementedInstruction(); break;
	case 0x43: UnimplementedInstruction(); break;
	case 0x44: UnimplementedInstruction(); break;
	case 0x45: UnimplementedInstruction(); break;
	case 0x46: UnimplementedInstruction(); break;
	case 0x47: UnimplementedInstruction(); break;
	case 0x48: UnimplementedInstruction(); break;
	case 0x49: UnimplementedInstruction(); break;
	case 0x4A: UnimplementedInstruction(); break;
	case 0x4B: UnimplementedInstruction(); break;
	case 0x4C: UnimplementedInstruction(); break;
	case 0x4D: UnimplementedInstruction(); break;
	case 0x4E: UnimplementedInstruction(); break;
	case 0x4F: UnimplementedInstruction(); break;
	case 0x50: UnimplementedInstruction(); break;
	case 0x51: UnimplementedInstruction(); break;
	case 0x52: UnimplementedInstruction(); break;
	case 0x53: UnimplementedInstruction(); break;
	case 0x54: UnimplementedInstruction(); break;
	case 0x55: UnimplementedInstruction(); break;
	case 0x56: UnimplementedInstruction(); break;
	case 0x57: UnimplementedInstruction(); break;
	case 0x58: UnimplementedInstruction(); break;
	case 0x59: UnimplementedInstruction(); break;
	case 0x5A: UnimplementedInstruction(); break;
	case 0x5B: UnimplementedInstruction(); break;
	case 0x5C: UnimplementedInstruction(); break;
	case 0x5D: UnimplementedInstruction(); break;
	case 0x5E: UnimplementedInstruction(); break;
	case 0x5F: UnimplementedInstruction(); break;
	case 0x60: UnimplementedInstruction(); break;
	case 0x61: UnimplementedInstruction(); break;
	case 0x62: UnimplementedInstruction(); break;
	case 0x63: UnimplementedInstruction(); break;
	case 0x64: UnimplementedInstruction(); break;
	case 0x65: UnimplementedInstruction(); break;
	case 0x66: UnimplementedInstruction(); break;
	case 0x67: UnimplementedInstruction(); break;
	case 0x68: UnimplementedInstruction(); break;
	case 0x69: UnimplementedInstruction(); break;
	case 0x6A: UnimplementedInstruction(); break;
	case 0x6B: UnimplementedInstruction(); break;
	case 0x6C: UnimplementedInstruction(); break;
	case 0x6D: UnimplementedInstruction(); break;
	case 0x6E: UnimplementedInstruction(); break;
	case 0x6F: UnimplementedInstruction(); break;
	case 0x70: UnimplementedInstruction(); break;
	case 0x71: UnimplementedInstruction(); break;
	case 0x72: UnimplementedInstruction(); break;
	case 0x73: UnimplementedInstruction(); break;
	case 0x74: UnimplementedInstruction(); break;
	case 0x75: UnimplementedInstruction(); break;
	case 0x76: UnimplementedInstruction(); break;
	case 0x77: UnimplementedInstruction(); break;
	case 0x78: UnimplementedInstruction(); break;
	case 0x79: UnimplementedInstruction(); break;
	case 0x7A: UnimplementedInstruction(); break;
	case 0x7B: UnimplementedInstruction(); break;
	case 0x7C: UnimplementedInstruction(); break;
	case 0x7D: UnimplementedInstruction(); break;
	case 0x7E: UnimplementedInstruction(); break;
	case 0x7F: UnimplementedInstruction(); break;
	case 0x80: UnimplementedInstruction(); break;
	case 0x81: UnimplementedInstruction(); break;
	case 0x82: UnimplementedInstruction(); break;
	case 0x83: UnimplementedInstruction(); break;
	case 0x84: UnimplementedInstruction(); break;
	case 0x85: UnimplementedInstruction(); break;
	case 0x86: UnimplementedInstruction(); break;
	case 0x87: UnimplementedInstruction(); break;
	case 0x88: UnimplementedInstruction(); break;
	case 0x89: UnimplementedInstruction(); break;
	case 0x8A: UnimplementedInstruction(); break;
	case 0x8B: UnimplementedInstruction(); break;
	case 0x8C: UnimplementedInstruction(); break;
	case 0x8D: UnimplementedInstruction(); break;
	case 0x8E: UnimplementedInstruction(); break;
	case 0x8F: UnimplementedInstruction(); break;
	case 0x90: UnimplementedInstruction(); break;
	case 0x91: UnimplementedInstruction(); break;
	case 0x92: UnimplementedInstruction(); break;
	case 0x93: UnimplementedInstruction(); break;
	case 0x94: UnimplementedInstruction(); break;
	case 0x95: UnimplementedInstruction(); break;
	case 0x96: UnimplementedInstruction(); break;
	case 0x97: UnimplementedInstruction(); break;
	case 0x98: UnimplementedInstruction(); break;
	case 0x99: UnimplementedInstruction(); break;
	case 0x9A: UnimplementedInstruction(); break;
	case 0x9B: UnimplementedInstruction(); break;
	case 0x9C: UnimplementedInstruction(); break;
	case 0x9D: UnimplementedInstruction(); break;
	case 0x9E: UnimplementedInstruction(); break;
	case 0x9F: UnimplementedInstruction(); break;
	case 0xA0: UnimplementedInstruction(); break;
	case 0xA1: UnimplementedInstruction(); break;
	case 0xA2: UnimplementedInstruction(); break;
	case 0xA3: UnimplementedInstruction(); break;
	case 0xA4: UnimplementedInstruction(); break;
	case 0xA5: UnimplementedInstruction(); break;
	case 0xA6: UnimplementedInstruction(); break;
	case 0xA7: UnimplementedInstruction(); break;
	case 0xA8: UnimplementedInstruction(); break;
	case 0xA9: UnimplementedInstruction(); break;
	case 0xAA: UnimplementedInstruction(); break;
	case 0xAB: UnimplementedInstruction(); break;
	case 0xAC: UnimplementedInstruction(); break;
	case 0xAD: UnimplementedInstruction(); break;
	case 0xAE: UnimplementedInstruction(); break;
	case 0xAF: UnimplementedInstruction(); break;
	case 0xB0: UnimplementedInstruction(); break;
	case 0xB1: UnimplementedInstruction(); break;
	case 0xB2: UnimplementedInstruction(); break;
	case 0xB3: UnimplementedInstruction(); break;
	case 0xB4: UnimplementedInstruction(); break;
	case 0xB5: UnimplementedInstruction(); break;
	case 0xB6: UnimplementedInstruction(); break;
	case 0xB7: UnimplementedInstruction(); break;
	case 0xB8: UnimplementedInstruction(); break;
	case 0xB9: UnimplementedInstruction(); break;
	case 0xBA: UnimplementedInstruction(); break;
	case 0xBB: UnimplementedInstruction(); break;
	case 0xBC: UnimplementedInstruction(); break;
	case 0xBD: UnimplementedInstruction(); break;
	case 0xBE: UnimplementedInstruction(); break;
	case 0xBF: UnimplementedInstruction(); break;
	case 0xC0: UnimplementedInstruction(); break;
	case 0xC1: UnimplementedInstruction(); break;
	case 0xC2: UnimplementedInstruction(); break;
	case 0xC3: UnimplementedInstruction(); break;
	case 0xC4: UnimplementedInstruction(); break;
	case 0xC5: UnimplementedInstruction(); break;
	case 0xC6: UnimplementedInstruction(); break;
	case 0xC7: UnimplementedInstruction(); break;
	case 0xC8: UnimplementedInstruction(); break;
	case 0xC9: UnimplementedInstruction(); break;
	case 0xCA: UnimplementedInstruction(); break;
	case 0xCB: UnimplementedInstruction(); break;
	case 0xCC: UnimplementedInstruction(); break;
	case 0xCD: UnimplementedInstruction(); break;
	case 0xCE: UnimplementedInstruction(); break;
	case 0xCF: UnimplementedInstruction(); break;
	case 0xD0: UnimplementedInstruction(); break;
	case 0xD1: UnimplementedInstruction(); break;
	case 0xD2: UnimplementedInstruction(); break;
	case 0xD3: UnimplementedInstruction(); break;
	case 0xD4: UnimplementedInstruction(); break;
	case 0xD5: UnimplementedInstruction(); break;
	case 0xD6: UnimplementedInstruction(); break;
	case 0xD7: UnimplementedInstruction(); break;
	case 0xD8: UnimplementedInstruction(); break;
	case 0xD9: UnimplementedInstruction(); break;
	case 0xDA: UnimplementedInstruction(); break;
	case 0xDB: UnimplementedInstruction(); break;
	case 0xDC: UnimplementedInstruction(); break;
	case 0xDD: UnimplementedInstruction(); break;
	case 0xDE: UnimplementedInstruction(); break;
	case 0xDF: UnimplementedInstruction(); break;
	case 0xE0: UnimplementedInstruction(); break;
	case 0xE1: UnimplementedInstruction(); break;
	case 0xE2: UnimplementedInstruction(); break;
	case 0xE3: UnimplementedInstruction(); break;
	case 0xE4: UnimplementedInstruction(); break;
	case 0xE5: UnimplementedInstruction(); break;
	case 0xE6: UnimplementedInstruction(); break;
	case 0xE7: UnimplementedInstruction(); break;
	case 0xE8: UnimplementedInstruction(); break;
	case 0xE9: UnimplementedInstruction(); break;
	case 0xEA: UnimplementedInstruction(); break;
	case 0xEB: UnimplementedInstruction(); break;
	case 0xEC: UnimplementedInstruction(); break;
	case 0xED: UnimplementedInstruction(); break;
	case 0xEE: UnimplementedInstruction(); break;
	case 0xEF: UnimplementedInstruction(); break;
	case 0xF0: UnimplementedInstruction(); break;
	case 0xF1: UnimplementedInstruction(); break;
	case 0xF2: UnimplementedInstruction(); break;
	case 0xF3: UnimplementedInstruction(); break;
	case 0xF4: UnimplementedInstruction(); break;
	case 0xF5: UnimplementedInstruction(); break;
	case 0xF6: UnimplementedInstruction(); break;
	case 0xF7: UnimplementedInstruction(); break;
	case 0xF8: UnimplementedInstruction(); break;
	case 0xF9: UnimplementedInstruction(); break;
	case 0xFA: UnimplementedInstruction(); break;
	case 0xFB: UnimplementedInstruction(); break;
	case 0xFC: UnimplementedInstruction(); break;
	case 0xFD: UnimplementedInstruction(); break;
	case 0xFE: UnimplementedInstruction(); break;
	case 0xFF: UnimplementedInstruction(); break;
	}
}

void CPU::ProcessCBInstruction()
{
	uint8_t opcode = this->memory->Read(this->PC);
	this->PC++;

	switch (opcode)
	{
	case 0x00: UnimplementedInstruction(); break;
	case 0x01: UnimplementedInstruction(); break;
	case 0x02: UnimplementedInstruction(); break;
	case 0x03: UnimplementedInstruction(); break;
	case 0x04: UnimplementedInstruction(); break;
	case 0x05: UnimplementedInstruction(); break;
	case 0x06: UnimplementedInstruction(); break;
	case 0x07: UnimplementedInstruction(); break;
	case 0x08: UnimplementedInstruction(); break;
	case 0x09: UnimplementedInstruction(); break;
	case 0x0A: UnimplementedInstruction(); break;
	case 0x0B: UnimplementedInstruction(); break;
	case 0x0C: UnimplementedInstruction(); break;
	case 0x0D: UnimplementedInstruction(); break;
	case 0x0E: UnimplementedInstruction(); break;
	case 0x0F: UnimplementedInstruction(); break;
	case 0x10: UnimplementedInstruction(); break;
	case 0x11: UnimplementedInstruction(); break;
	case 0x12: UnimplementedInstruction(); break;
	case 0x13: UnimplementedInstruction(); break;
	case 0x14: UnimplementedInstruction(); break;
	case 0x15: UnimplementedInstruction(); break;
	case 0x16: UnimplementedInstruction(); break;
	case 0x17: UnimplementedInstruction(); break;
	case 0x18: UnimplementedInstruction(); break;
	case 0x19: UnimplementedInstruction(); break;
	case 0x1A: UnimplementedInstruction(); break;
	case 0x1B: UnimplementedInstruction(); break;
	case 0x1C: UnimplementedInstruction(); break;
	case 0x1D: UnimplementedInstruction(); break;
	case 0x1E: UnimplementedInstruction(); break;
	case 0x1F: UnimplementedInstruction(); break;
	case 0x20: UnimplementedInstruction(); break;
	case 0x21: UnimplementedInstruction(); break;
	case 0x22: UnimplementedInstruction(); break;
	case 0x23: UnimplementedInstruction(); break;
	case 0x24: UnimplementedInstruction(); break;
	case 0x25: UnimplementedInstruction(); break;
	case 0x26: UnimplementedInstruction(); break;
	case 0x27: UnimplementedInstruction(); break;
	case 0x28: UnimplementedInstruction(); break;
	case 0x29: UnimplementedInstruction(); break;
	case 0x2A: UnimplementedInstruction(); break;
	case 0x2B: UnimplementedInstruction(); break;
	case 0x2C: UnimplementedInstruction(); break;
	case 0x2D: UnimplementedInstruction(); break;
	case 0x2E: UnimplementedInstruction(); break;
	case 0x2F: UnimplementedInstruction(); break;
	case 0x30: UnimplementedInstruction(); break;
	case 0x31: UnimplementedInstruction(); break;
	case 0x32: UnimplementedInstruction(); break;
	case 0x33: UnimplementedInstruction(); break;
	case 0x34: UnimplementedInstruction(); break;
	case 0x35: UnimplementedInstruction(); break;
	case 0x36: UnimplementedInstruction(); break;
	case 0x37: UnimplementedInstruction(); break;
	case 0x38: UnimplementedInstruction(); break;
	case 0x39: UnimplementedInstruction(); break;
	case 0x3A: UnimplementedInstruction(); break;
	case 0x3B: UnimplementedInstruction(); break;
	case 0x3C: UnimplementedInstruction(); break;
	case 0x3D: UnimplementedInstruction(); break;
	case 0x3E: UnimplementedInstruction(); break;
	case 0x3F: UnimplementedInstruction(); break;
	case 0x40: UnimplementedInstruction(); break;
	case 0x41: UnimplementedInstruction(); break;
	case 0x42: UnimplementedInstruction(); break;
	case 0x43: UnimplementedInstruction(); break;
	case 0x44: UnimplementedInstruction(); break;
	case 0x45: UnimplementedInstruction(); break;
	case 0x46: UnimplementedInstruction(); break;
	case 0x47: UnimplementedInstruction(); break;
	case 0x48: UnimplementedInstruction(); break;
	case 0x49: UnimplementedInstruction(); break;
	case 0x4A: UnimplementedInstruction(); break;
	case 0x4B: UnimplementedInstruction(); break;
	case 0x4C: UnimplementedInstruction(); break;
	case 0x4D: UnimplementedInstruction(); break;
	case 0x4E: UnimplementedInstruction(); break;
	case 0x4F: UnimplementedInstruction(); break;
	case 0x50: UnimplementedInstruction(); break;
	case 0x51: UnimplementedInstruction(); break;
	case 0x52: UnimplementedInstruction(); break;
	case 0x53: UnimplementedInstruction(); break;
	case 0x54: UnimplementedInstruction(); break;
	case 0x55: UnimplementedInstruction(); break;
	case 0x56: UnimplementedInstruction(); break;
	case 0x57: UnimplementedInstruction(); break;
	case 0x58: UnimplementedInstruction(); break;
	case 0x59: UnimplementedInstruction(); break;
	case 0x5A: UnimplementedInstruction(); break;
	case 0x5B: UnimplementedInstruction(); break;
	case 0x5C: UnimplementedInstruction(); break;
	case 0x5D: UnimplementedInstruction(); break;
	case 0x5E: UnimplementedInstruction(); break;
	case 0x5F: UnimplementedInstruction(); break;
	case 0x60: UnimplementedInstruction(); break;
	case 0x61: UnimplementedInstruction(); break;
	case 0x62: UnimplementedInstruction(); break;
	case 0x63: UnimplementedInstruction(); break;
	case 0x64: UnimplementedInstruction(); break;
	case 0x65: UnimplementedInstruction(); break;
	case 0x66: UnimplementedInstruction(); break;
	case 0x67: UnimplementedInstruction(); break;
	case 0x68: UnimplementedInstruction(); break;
	case 0x69: UnimplementedInstruction(); break;
	case 0x6A: UnimplementedInstruction(); break;
	case 0x6B: UnimplementedInstruction(); break;
	case 0x6C: UnimplementedInstruction(); break;
	case 0x6D: UnimplementedInstruction(); break;
	case 0x6E: UnimplementedInstruction(); break;
	case 0x6F: UnimplementedInstruction(); break;
	case 0x70: UnimplementedInstruction(); break;
	case 0x71: UnimplementedInstruction(); break;
	case 0x72: UnimplementedInstruction(); break;
	case 0x73: UnimplementedInstruction(); break;
	case 0x74: UnimplementedInstruction(); break;
	case 0x75: UnimplementedInstruction(); break;
	case 0x76: UnimplementedInstruction(); break;
	case 0x77: UnimplementedInstruction(); break;
	case 0x78: UnimplementedInstruction(); break;
	case 0x79: UnimplementedInstruction(); break;
	case 0x7A: UnimplementedInstruction(); break;
	case 0x7B: UnimplementedInstruction(); break;
	case 0x7C: UnimplementedInstruction(); break;
	case 0x7D: UnimplementedInstruction(); break;
	case 0x7E: UnimplementedInstruction(); break;
	case 0x7F: UnimplementedInstruction(); break;
	case 0x80: UnimplementedInstruction(); break;
	case 0x81: UnimplementedInstruction(); break;
	case 0x82: UnimplementedInstruction(); break;
	case 0x83: UnimplementedInstruction(); break;
	case 0x84: UnimplementedInstruction(); break;
	case 0x85: UnimplementedInstruction(); break;
	case 0x86: UnimplementedInstruction(); break;
	case 0x87: UnimplementedInstruction(); break;
	case 0x88: UnimplementedInstruction(); break;
	case 0x89: UnimplementedInstruction(); break;
	case 0x8A: UnimplementedInstruction(); break;
	case 0x8B: UnimplementedInstruction(); break;
	case 0x8C: UnimplementedInstruction(); break;
	case 0x8D: UnimplementedInstruction(); break;
	case 0x8E: UnimplementedInstruction(); break;
	case 0x8F: UnimplementedInstruction(); break;
	case 0x90: UnimplementedInstruction(); break;
	case 0x91: UnimplementedInstruction(); break;
	case 0x92: UnimplementedInstruction(); break;
	case 0x93: UnimplementedInstruction(); break;
	case 0x94: UnimplementedInstruction(); break;
	case 0x95: UnimplementedInstruction(); break;
	case 0x96: UnimplementedInstruction(); break;
	case 0x97: UnimplementedInstruction(); break;
	case 0x98: UnimplementedInstruction(); break;
	case 0x99: UnimplementedInstruction(); break;
	case 0x9A: UnimplementedInstruction(); break;
	case 0x9B: UnimplementedInstruction(); break;
	case 0x9C: UnimplementedInstruction(); break;
	case 0x9D: UnimplementedInstruction(); break;
	case 0x9E: UnimplementedInstruction(); break;
	case 0x9F: UnimplementedInstruction(); break;
	case 0xA0: UnimplementedInstruction(); break;
	case 0xA1: UnimplementedInstruction(); break;
	case 0xA2: UnimplementedInstruction(); break;
	case 0xA3: UnimplementedInstruction(); break;
	case 0xA4: UnimplementedInstruction(); break;
	case 0xA5: UnimplementedInstruction(); break;
	case 0xA6: UnimplementedInstruction(); break;
	case 0xA7: UnimplementedInstruction(); break;
	case 0xA8: UnimplementedInstruction(); break;
	case 0xA9: UnimplementedInstruction(); break;
	case 0xAA: UnimplementedInstruction(); break;
	case 0xAB: UnimplementedInstruction(); break;
	case 0xAC: UnimplementedInstruction(); break;
	case 0xAD: UnimplementedInstruction(); break;
	case 0xAE: UnimplementedInstruction(); break;
	case 0xAF: UnimplementedInstruction(); break;
	case 0xB0: UnimplementedInstruction(); break;
	case 0xB1: UnimplementedInstruction(); break;
	case 0xB2: UnimplementedInstruction(); break;
	case 0xB3: UnimplementedInstruction(); break;
	case 0xB4: UnimplementedInstruction(); break;
	case 0xB5: UnimplementedInstruction(); break;
	case 0xB6: UnimplementedInstruction(); break;
	case 0xB7: UnimplementedInstruction(); break;
	case 0xB8: UnimplementedInstruction(); break;
	case 0xB9: UnimplementedInstruction(); break;
	case 0xBA: UnimplementedInstruction(); break;
	case 0xBB: UnimplementedInstruction(); break;
	case 0xBC: UnimplementedInstruction(); break;
	case 0xBD: UnimplementedInstruction(); break;
	case 0xBE: UnimplementedInstruction(); break;
	case 0xBF: UnimplementedInstruction(); break;
	case 0xC0: UnimplementedInstruction(); break;
	case 0xC1: UnimplementedInstruction(); break;
	case 0xC2: UnimplementedInstruction(); break;
	case 0xC3: UnimplementedInstruction(); break;
	case 0xC4: UnimplementedInstruction(); break;
	case 0xC5: UnimplementedInstruction(); break;
	case 0xC6: UnimplementedInstruction(); break;
	case 0xC7: UnimplementedInstruction(); break;
	case 0xC8: UnimplementedInstruction(); break;
	case 0xC9: UnimplementedInstruction(); break;
	case 0xCA: UnimplementedInstruction(); break;
	case 0xCB: UnimplementedInstruction(); break;
	case 0xCC: UnimplementedInstruction(); break;
	case 0xCD: UnimplementedInstruction(); break;
	case 0xCE: UnimplementedInstruction(); break;
	case 0xCF: UnimplementedInstruction(); break;
	case 0xD0: UnimplementedInstruction(); break;
	case 0xD1: UnimplementedInstruction(); break;
	case 0xD2: UnimplementedInstruction(); break;
	case 0xD3: UnimplementedInstruction(); break;
	case 0xD4: UnimplementedInstruction(); break;
	case 0xD5: UnimplementedInstruction(); break;
	case 0xD6: UnimplementedInstruction(); break;
	case 0xD7: UnimplementedInstruction(); break;
	case 0xD8: UnimplementedInstruction(); break;
	case 0xD9: UnimplementedInstruction(); break;
	case 0xDA: UnimplementedInstruction(); break;
	case 0xDB: UnimplementedInstruction(); break;
	case 0xDC: UnimplementedInstruction(); break;
	case 0xDD: UnimplementedInstruction(); break;
	case 0xDE: UnimplementedInstruction(); break;
	case 0xDF: UnimplementedInstruction(); break;
	case 0xE0: UnimplementedInstruction(); break;
	case 0xE1: UnimplementedInstruction(); break;
	case 0xE2: UnimplementedInstruction(); break;
	case 0xE3: UnimplementedInstruction(); break;
	case 0xE4: UnimplementedInstruction(); break;
	case 0xE5: UnimplementedInstruction(); break;
	case 0xE6: UnimplementedInstruction(); break;
	case 0xE7: UnimplementedInstruction(); break;
	case 0xE8: UnimplementedInstruction(); break;
	case 0xE9: UnimplementedInstruction(); break;
	case 0xEA: UnimplementedInstruction(); break;
	case 0xEB: UnimplementedInstruction(); break;
	case 0xEC: UnimplementedInstruction(); break;
	case 0xED: UnimplementedInstruction(); break;
	case 0xEE: UnimplementedInstruction(); break;
	case 0xEF: UnimplementedInstruction(); break;
	case 0xF0: UnimplementedInstruction(); break;
	case 0xF1: UnimplementedInstruction(); break;
	case 0xF2: UnimplementedInstruction(); break;
	case 0xF3: UnimplementedInstruction(); break;
	case 0xF4: UnimplementedInstruction(); break;
	case 0xF5: UnimplementedInstruction(); break;
	case 0xF6: UnimplementedInstruction(); break;
	case 0xF7: UnimplementedInstruction(); break;
	case 0xF8: UnimplementedInstruction(); break;
	case 0xF9: UnimplementedInstruction(); break;
	case 0xFA: UnimplementedInstruction(); break;
	case 0xFB: UnimplementedInstruction(); break;
	case 0xFC: UnimplementedInstruction(); break;
	case 0xFD: UnimplementedInstruction(); break;
	case 0xFE: UnimplementedInstruction(); break;
	case 0xFF: UnimplementedInstruction(); break;
	}
}
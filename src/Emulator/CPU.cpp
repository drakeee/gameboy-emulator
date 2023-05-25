#include <Main.h>

using namespace GameBoy;

const std::array<uint8_t, 256> CPU::boot_rom = {
	// Offset 0x00000000 to 0x000000FF
	0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB,
	0x21, 0x26, 0xFF, 0x0E, 0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3,
	0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, 0x47, 0x11, 0x04, 0x01,
	0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
	0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22,
	0x23, 0x05, 0x20, 0xF9, 0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99,
	0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, 0xF9, 0x2E, 0x0F, 0x18,
	0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
	0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20,
	0xF7, 0x1D, 0x20, 0xF2, 0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62,
	0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, 0x7B, 0xE2, 0x0C, 0x3E,
	0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
	0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17,
	0xC1, 0xCB, 0x11, 0x17, 0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9,
	0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
	0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
	0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
	0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
	0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, 0x21, 0x04, 0x01, 0x11,
	0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
	0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE,
	0x3E, 0x01, 0xE0, 0x50
};

CPU::CPU(Emulator* emulator) :
	emulator(emulator)
{
	fopen_s(&debugFile, "debug.log", "w+");
	//printf("memory: %d\n", this->emulator->memory);
}

CPU::~CPU()
{

}

void CPU::UnimplementedInstruction(const char* msg)
{
	uint8_t opcode = this->emulator->memory->Read(this->PC - 1);
	printf("UnimplementedInstruction: OpCode: %02X, %s\n", opcode, msg);
}

/*
UnimplementedInstruction: OpCode: F3,
UnimplementedInstruction: OpCode: EA,
UnimplementedInstruction: OpCode: E0,
UnimplementedInstruction: OpCode: E0,
UnimplementedInstruction: OpCode: E0,
*/

void CPU::HandleInterrupts()
{
	if (!interrupts_enabled)
		return;

	printf("Interrupt handling!");
}

uint32_t cycles = 0;
uint32_t cyclesProcessed = 0;
void CPU::ProcessInstruction()
{
	if (this->emulator->memory == nullptr)
		return;

	fprintf(debugFile, "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
		this->AF.msb,
		this->AF.lsb,
		this->BC.msb,
		this->BC.lsb,
		this->DE.msb,
		this->DE.lsb,
		this->HL.msb,
		this->HL.lsb,
		this->SP,
		this->PC,
		this->emulator->memory->Read(this->PC),
		this->emulator->memory->Read(this->PC + 1),
		this->emulator->memory->Read(this->PC + 2),
		this->emulator->memory->Read(this->PC + 3));
	fflush(debugFile);

	HandleInterrupts();

	if (debug)
	{
		getchar();
	}

	/*if (cycles == 0)
	{
		std::cout << "Type a number to process: ";
		std::cin >> cycles;
	}
	else
	{
		cyclesProcessed++;

		if (cyclesProcessed == cycles)
		{
			cycles = 0;
			cyclesProcessed = 0;
		}
	}*/


	uint8_t opcode = this->emulator->memory->Read(this->PC);
	if (opcode == 0xEC)
	{
		uint8_t opcode1 = this->emulator->memory->Read(this->PC - 2);
		printf("HAHA! %02X\n", opcode1);
		//debug = true;
	}

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
	case 0x17: this->opcode_rl(this->AF.msb); break; //RLA
	case 0x18: this->opcode_jr(Condition::NONE); break; //JR s8
	case 0x19: this->opcode_add(this->HL, this->DE); break; //ADD HL, DE
	case 0x1A: this->opcode_ld(this->AF.msb, this->DE); break; //LD A, (DE)
	case 0x1B: this->opcode_dec(this->DE); break; //DEC DE
	case 0x1C: this->opcode_inc(this->DE.lsb); break; //INC E
	case 0x1D: this->opcode_dec(this->DE.lsb); break; //DEC E
	case 0x1E: this->opcode_ld(this->DE.lsb); break; //LD E, d8
	case 0x1F: this->opcode_rr(this->AF.msb); break; //RRA
	case 0x20: this->opcode_jr(Condition::NOT_ZERO); break; //JR NZ, s8
	case 0x21: this->opcode_ld(this->HL); break; //LD HL, d16
	case 0x22: this->opcode_ld(this->HL++, this->AF.msb); break; //LD (HL+), A
	case 0x23: this->opcode_inc(this->HL); break; //INC HL
	case 0x24: this->opcode_inc(this->HL.msb); break; //INC H
	case 0x25: this->opcode_dec(this->HL.msb); break; //DEC H
	case 0x26: this->opcode_ld(this->HL.msb); break; //LD H
	case 0x27: this->opcode_daa(); break; //TODO: DAA
	case 0x28: this->opcode_jr(Condition::ZERO); break; //JR Z, s8
	case 0x29: this->opcode_add(this->HL, this->HL); break; //ADD HL, HL
	case 0x2A: this->opcode_ld(this->AF.msb, this->emulator->memory->Read(this->HL)); this->HL++; break; //LD A, (HL+)
	case 0x2B: this->opcode_dec(this->HL); break; //DEC HL
	case 0x2C: this->opcode_inc(this->HL.lsb); break; //INC L
	case 0x2D: this->opcode_dec(this->HL.lsb); break; //DEC L
	case 0x2E: this->opcode_ld(this->HL.lsb); break; //LD L, d8
	case 0x2F: this->opcode_cpl(); break; //CPL
	case 0x30: this->opcode_jr(Condition::NOT_CARRY); break; //JR NC, s8
	case 0x31: this->opcode_ld(this->SP); break; //LD SP, d16
	case 0x32: this->opcode_ld(this->HL--, this->AF.msb); break; //LD (HL-), A
	case 0x33: this->opcode_inc(this->SP); break; //INC SP
	case 0x34: this->opcode_inc_memory(this->HL); break; //INC (HL)
	case 0x35: this->opcode_dec_memory(this->HL); break; //DEC (HL)
	case 0x36: this->opcode_ld_memory(this->HL); break; //LD (HL), d8
	case 0x37: this->opcode_scf(); break; //SCF
	case 0x38: this->opcode_jr(Condition::CARRY); break; //JR C, s8
	case 0x39: this->opcode_add(this->HL, this->SP); break; //ADD HL, SP
	case 0x3A: this->opcode_ld(this->AF.msb, this->emulator->memory->Read(this->HL)); this->HL--; break; //LD A, HL-
	case 0x3B: this->opcode_dec(this->SP); break; //DEC SP
	case 0x3C: this->opcode_inc(this->AF.msb); break; //INC A
	case 0x3D: this->opcode_dec(this->AF.msb); break; //DEC A
	case 0x3E: this->opcode_ld(this->AF.msb); break; //LD A, d8
	case 0x3F: this->opcode_ccf(); break; //CCF
	case 0x40: this->opcode_ld(this->BC.msb, this->BC.msb); break; //LD B, B
	case 0x41: this->opcode_ld(this->BC.msb, this->BC.lsb); break; //LD B, C
	case 0x42: this->opcode_ld(this->BC.msb, this->DE.msb); break; //LD B, D
	case 0x43: this->opcode_ld(this->BC.msb, this->DE.lsb); break; //LD B, E
	case 0x44: this->opcode_ld(this->BC.msb, this->HL.msb); break; //LD B, H
	case 0x45: this->opcode_ld(this->BC.msb, this->HL.lsb); break; //LD B, L
	case 0x46: this->opcode_ld(this->BC.msb, this->HL); break; //LD B, (HL)
	case 0x47: this->opcode_ld(this->BC.msb, this->AF.msb); break; //LD B, A
	case 0x48: this->opcode_ld(this->BC.lsb, this->BC.msb); break; //LD C, B
	case 0x49: this->opcode_ld(this->BC.lsb, this->BC.lsb); break; //LD C, C
	case 0x4A: this->opcode_ld(this->BC.lsb, this->DE.msb); break; //LD C, D
	case 0x4B: this->opcode_ld(this->BC.lsb, this->DE.lsb); break; //LD C, E
	case 0x4C: this->opcode_ld(this->BC.lsb, this->HL.msb); break; //LD C, H
	case 0x4D: this->opcode_ld(this->BC.lsb, this->HL.lsb); break; //LD C, L
	case 0x4E: this->opcode_ld(this->BC.lsb, this->HL); break; //LD C, (HL)
	case 0x4F: this->opcode_ld(this->BC.lsb, this->AF.msb); break; //LD C, A
	case 0x50: this->opcode_ld(this->DE.msb, this->BC.msb); break; //LD D, B
	case 0x51: this->opcode_ld(this->DE.msb, this->BC.lsb); break; //LD D, C
	case 0x52: this->opcode_ld(this->DE.msb, this->DE.msb); break; //LD D, D
	case 0x53: this->opcode_ld(this->DE.msb, this->DE.lsb); break; //LD D, E
	case 0x54: this->opcode_ld(this->DE.msb, this->HL.msb); break; //LD D, H
	case 0x55: this->opcode_ld(this->DE.msb, this->HL.lsb); break; //LD D, L
	case 0x56: this->opcode_ld(this->DE.msb, this->HL); break; //LD D, (HL)
	case 0x57: this->opcode_ld(this->DE.msb, this->AF.msb); break; //LD D, A
	case 0x58: this->opcode_ld(this->DE.lsb, this->BC.msb); break; //LD E, B
	case 0x59: this->opcode_ld(this->DE.lsb, this->BC.lsb); break; //LD E, C
	case 0x5A: this->opcode_ld(this->DE.lsb, this->DE.msb); break; //LD E, D
	case 0x5B: this->opcode_ld(this->DE.lsb, this->DE.lsb); break; //LD E, E
	case 0x5C: this->opcode_ld(this->DE.lsb, this->HL.msb); break; //LD E, H
	case 0x5D: this->opcode_ld(this->DE.lsb, this->HL.lsb); break; //LD E, L
	case 0x5E: this->opcode_ld(this->DE.lsb, this->HL); break; //LD E, (HL)
	case 0x5F: this->opcode_ld(this->DE.lsb, this->AF.msb); break; //LD E, A
	case 0x60: this->opcode_ld(this->HL.msb, this->BC.msb); break; //LD H, B
	case 0x61: this->opcode_ld(this->HL.msb, this->BC.lsb); break; //LD H, C
	case 0x62: this->opcode_ld(this->HL.msb, this->DE.msb); break; //LD H, D
	case 0x63: this->opcode_ld(this->HL.msb, this->DE.lsb); break; //LD H, E
	case 0x64: this->opcode_ld(this->HL.msb, this->HL.msb); break; //LD H, H
	case 0x65: this->opcode_ld(this->HL.msb, this->HL.lsb); break; //LD H, L
	case 0x66: this->opcode_ld(this->HL.msb, this->HL); break; //LD H, (HL)
	case 0x67: this->opcode_ld(this->HL.msb, this->AF.msb); break; //LD H, A
	case 0x68: this->opcode_ld(this->HL.lsb, this->BC.msb); break; //LD L, B
	case 0x69: this->opcode_ld(this->HL.lsb, this->BC.lsb); break; //LD L, C
	case 0x6A: this->opcode_ld(this->HL.lsb, this->DE.msb); break; // LD L, D
	case 0x6B: this->opcode_ld(this->HL.lsb, this->DE.lsb); break; //LD L, E
	case 0x6C: this->opcode_ld(this->HL.lsb, this->HL.msb); break; //LD L, H
	case 0x6D: this->opcode_ld(this->HL.lsb, this->HL.lsb); break; //LD L, L
	case 0x6E: this->opcode_ld(this->HL.lsb, this->HL); break; //LD L, (HL)
	case 0x6F: this->opcode_ld(this->HL.lsb, this->AF.msb); break; //LD L, A
	case 0x70: this->opcode_ld(this->HL, this->BC.msb); break; //LD (HL), B
	case 0x71: this->opcode_ld(this->HL, this->BC.lsb); break; //LD (HL), C
	case 0x72: this->opcode_ld(this->HL, this->DE.msb); break; //LD (HL), D
	case 0x73: this->opcode_ld(this->HL, this->DE.lsb); break; //LD (HL), E
	case 0x74: this->opcode_ld(this->HL, this->HL.msb); break; //LD (HL), H
	case 0x75: this->opcode_ld(this->HL, this->HL.lsb); break; //LD (HL), L
	case 0x76: UnimplementedInstruction(); break; //HALT
	case 0x77: this->opcode_ld(this->HL, this->AF.msb); break; //LD (HL), A
	case 0x78: this->opcode_ld(this->AF.msb, this->BC.msb); break; //LD A, B
	case 0x79: this->opcode_ld(this->AF.msb, this->BC.lsb); break; //LD A, C
	case 0x7A: this->opcode_ld(this->AF.msb, this->DE.msb); break; //LD A, D
	case 0x7B: this->opcode_ld(this->AF.msb, this->DE.lsb); break; //LD A, E
	case 0x7C: this->opcode_ld(this->AF.msb, this->HL.msb); break; //LD A, H
	case 0x7D: this->opcode_ld(this->AF.msb, this->HL.lsb); break; //LD A, L
	case 0x7E: this->opcode_ld(this->AF.msb, this->HL); break; //LD A, (HL)
	case 0x7F: this->opcode_ld(this->AF.msb, this->AF.msb); break; //LD A, A
	case 0x80: this->opcode_add(this->AF.msb, this->BC.msb); break; //ADD A, B
	case 0x81: this->opcode_add(this->AF.msb, this->BC.lsb); break; //ADD A, C
	case 0x82: this->opcode_add(this->AF.msb, this->DE.msb); break; //ADD A, D
	case 0x83: this->opcode_add(this->AF.msb, this->DE.lsb); break; //ADD A, E
	case 0x84: this->opcode_add(this->AF.msb, this->HL.msb); break; //ADD A, H
	case 0x85: this->opcode_add(this->AF.msb, this->HL.lsb); break; //ADD A, L
	case 0x86: this->opcode_add(this->AF.msb, this->HL); break; //ADD A, (HL)
	case 0x87: this->opcode_add(this->AF.msb, this->AF.msb); break; //ADD A, A
	case 0x88: this->opcode_adc(this->AF.msb, this->BC.msb); break; //ADC A, B
	case 0x89: this->opcode_adc(this->AF.msb, this->BC.lsb); break; //ADC A, C
	case 0x8A: this->opcode_adc(this->AF.msb, this->DE.msb); break; //ADC A, D
	case 0x8B: this->opcode_adc(this->AF.msb, this->DE.lsb); break; //ADC A, E
	case 0x8C: this->opcode_adc(this->AF.msb, this->HL.msb); break; //ADC A, H
	case 0x8D: this->opcode_adc(this->AF.msb, this->HL.lsb); break; //ADC A, L
	case 0x8E: this->opcode_adc(this->AF.msb, this->HL); break; //ADC A, (HL)
	case 0x8F: this->opcode_adc(this->AF.msb, this->AF.msb); break; //ADC A, A
	case 0x90: this->opcode_sub(this->BC.msb); break; //SUB B
	case 0x91: this->opcode_sub(this->BC.lsb); break; //SUB C
	case 0x92: this->opcode_sub(this->DE.msb); break; //SUB D
	case 0x93: this->opcode_sub(this->DE.lsb); break; //SUB E
	case 0x94: this->opcode_sub(this->HL.msb); break; //SUB H
	case 0x95: this->opcode_sub(this->HL.lsb); break; //SUB L
	case 0x96: this->opcode_sub(this->HL); break; //SUB (HL)
	case 0x97: this->opcode_sub(this->AF.msb); break; //SUB A
	case 0x98: this->opcode_sbc(this->AF.msb, this->BC.msb); break; //SBC A, B
	case 0x99: this->opcode_sbc(this->AF.msb, this->BC.lsb); break; //SBC A, C
	case 0x9A: this->opcode_sbc(this->AF.msb, this->DE.msb); break; //SBC A, D
	case 0x9B: this->opcode_sbc(this->AF.msb, this->DE.lsb); break; //SBC A, E
	case 0x9C: this->opcode_sbc(this->AF.msb, this->HL.msb); break; //SBC A, H
	case 0x9D: this->opcode_sbc(this->AF.msb, this->HL.lsb); break; //SBC A, L
	case 0x9E: this->opcode_sbc(this->AF.msb, this->HL); break; //SBC A, (HL)
	case 0x9F: this->opcode_sbc(this->AF.msb, this->AF.msb); break; //SBC A, A
	case 0xA0: this->opcode_and(this->BC.msb); break; //AND B
	case 0xA1: this->opcode_and(this->BC.lsb); break; //AND C
	case 0xA2: this->opcode_and(this->DE.msb); break; //AND D
	case 0xA3: this->opcode_and(this->DE.lsb); break; //AND E
	case 0xA4: this->opcode_and(this->HL.msb); break; //AND H
	case 0xA5: this->opcode_and(this->HL.lsb); break; //AND L
	case 0xA6: this->opcode_and(this->HL); break; //AND (HL)
	case 0xA7: this->opcode_and(this->AF.msb); break; //AND A
	case 0xA8: this->opcode_xor(this->BC.msb); break; //XOR B
	case 0xA9: this->opcode_xor(this->BC.lsb); break; //XOR C
	case 0xAA: this->opcode_xor(this->DE.msb); break; //XOR D
	case 0xAB: this->opcode_xor(this->DE.lsb); break; //XOR E
	case 0xAC: this->opcode_xor(this->HL.msb); break; //XOR H
	case 0xAD: this->opcode_xor(this->HL.lsb); break; //XOR L
	case 0xAE: this->opcode_xor(this->HL); break; //XOR (HL)
	case 0xAF: this->opcode_xor(this->AF.msb); break; //XOR A
	case 0xB0: this->opcode_or(this->BC.msb); break; //OR B
	case 0xB1: this->opcode_or(this->BC.lsb); break; //OR C
	case 0xB2: this->opcode_or(this->DE.msb); break; //OR D
	case 0xB3: this->opcode_or(this->DE.lsb); break; //OR E
	case 0xB4: this->opcode_or(this->HL.msb); break; //OR H
	case 0xB5: this->opcode_or(this->HL.lsb); break; //OR L
	case 0xB6: this->opcode_or(this->HL); break; //OR (HL)
	case 0xB7: this->opcode_or(this->AF.msb); break; //OR A
	case 0xB8: this->opcode_cp(this->BC.msb); break; //CP B
	case 0xB9: this->opcode_cp(this->BC.lsb); break; //CP C
	case 0xBA: this->opcode_cp(this->DE.msb); break; //CP D
	case 0xBB: this->opcode_cp(this->DE.lsb); break; //CP E
	case 0xBC: this->opcode_cp(this->HL.msb); break; //CP H
	case 0xBD: this->opcode_cp(this->HL.lsb); break; //CP L
	case 0xBE: this->opcode_cp(this->HL); break; //CP (HL)
	case 0xBF: this->opcode_cp(this->AF.msb); break; //CP A
	case 0xC0: this->opcode_ret(Condition::NOT_ZERO); break; //RET NZ
	case 0xC1: this->opcode_pop(this->BC); break; //POP BC
	case 0xC2: this->opcode_jp(Condition::NOT_ZERO); break; //JP NZ, a16
	case 0xC3: this->opcode_jp(); break; //JP a16
	case 0xC4: this->opcode_call(Condition::NOT_ZERO); break; //CALL NZ, a16
	case 0xC5: this->opcode_push(this->BC); break; //PUSH BC
	case 0xC6: this->opcode_add(this->AF.msb, this->read_byte()); break; //ADD A, d8
	case 0xC7: this->opcode_rst(0x00); break; //RST 00h
	case 0xC8: this->opcode_ret(Condition::ZERO); break; //RET Z
	case 0xC9: this->opcode_ret(); break; //RET
	case 0xCA: this->opcode_jp(Condition::ZERO); break; //JP Z, a16
	case 0xCB: ProcessCBInstruction(); break; //PREFIX CB
	case 0xCC: this->opcode_call(Condition::ZERO); break; //CALL Z, a16
	case 0xCD: this->opcode_call(); break; //CALL a16
	case 0xCE: this->opcode_adc(this->AF.msb, this->read_byte()); break; //ADC A, d8
	case 0xCF: this->opcode_rst(0x08); break; //RST 08h
	case 0xD0: this->opcode_ret(Condition::NOT_CARRY); break; //RET NC
	case 0xD1: this->opcode_pop(this->DE); break; //POP DE
	case 0xD2: this->opcode_jp(Condition::NOT_CARRY); break; //JP NC, a16
	case 0xD3: UnimplementedInstruction(); break; //NOT USED
	case 0xD4: this->opcode_call(Condition::NOT_CARRY); break; //CALL NC, a16
	case 0xD5: this->opcode_push(this->DE); break; //PUSH DE
	case 0xD6: this->opcode_sub(this->read_byte()); break; //SUB d8
	case 0xD7: this->opcode_rst(0x10); break; //RST 10h
	case 0xD8: this->opcode_ret(Condition::CARRY); break; //RET C
	case 0xD9: this->opcode_ret(); this->opcode_ei(); break; //RETI
	case 0xDA: this->opcode_jp(Condition::CARRY); break; //JP C, a16
	case 0xDB: UnimplementedInstruction(); break; //NOT USED
	case 0xDC: this->opcode_call(Condition::CARRY); break; //CALL C, a16
	case 0xDD: UnimplementedInstruction(); break; //NOT USED
	case 0xDE: this->opcode_sbc(this->AF.msb, this->read_byte()); break; //SBC A, d8
	case 0xDF: this->opcode_rst(0x18); break; //RST 18h
	case 0xE0: this->opcode_ldc_a_into_a8(); break; //LDH (a8), A
	case 0xE1: this->opcode_pop(this->HL); break; //POP HL
	case 0xE2: this->opcode_ldc_a_into_c(); break; //LDH (C), A
	case 0xE3: UnimplementedInstruction(); break;
	case 0xE4: UnimplementedInstruction(); break;
	case 0xE5: this->opcode_push(this->HL); break; //PUSH HL
	case 0xE6: this->opcode_and(this->read_byte()); break; //AND d8
	case 0xE7: this->opcode_rst(0x20); break; //RST 20h
	case 0xE8: this->opcode_add_sp(); break; //ADD SP, r8
	case 0xE9: this->opcode_jp_hl(); break; //JP HL - could be JP (HL)
	case 0xEA: this->opcode_ld(this->read_word(), this->AF.msb); break; //LD (a16), A
	case 0xEB: UnimplementedInstruction(); break;
	case 0xEC: UnimplementedInstruction(); break;
	case 0xED: UnimplementedInstruction(); break;
	case 0xEE: this->opcode_xor(this->read_byte()); break; //XOR d8
	case 0xEF: this->opcode_rst(0x28); break; //RST 28h
	case 0xF0: this->opcode_ldc_a8_into_a(); break; //LDH A, (a8)
	case 0xF1: this->opcode_pop(this->AF); this->AF.lsb &= 0xF0; break; //POP AF
	case 0xF2: this->opcode_ldc_c_into_a(); break; //LDH A, (C)
	case 0xF3: this->opcode_di(); break; //DI
	case 0xF4: UnimplementedInstruction(); break;
	case 0xF5: this->opcode_push(this->AF); break; //PUSH AF
	case 0xF6: this->opcode_or(this->read_byte()); break; //OR d8
	case 0xF7: this->opcode_rst(0x30); break; //RST 30h
	case 0xF8: this->opcode_ld_sp_into_hl(); break; //LD HL, SP+r8
	case 0xF9: this->SP = this->HL; break; //LD SP, HL
	case 0xFA: this->opcode_ld(this->AF.msb, this->emulator->memory->Read(this->read_word())); break;
	case 0xFB: this->opcode_ei(); break; //EI
	case 0xFC: UnimplementedInstruction(); break;
	case 0xFD: UnimplementedInstruction(); break;
	case 0xFE: this->opcode_cp(this->read_byte()); break; //CP d8
	case 0xFF: this->opcode_rst(0x38); break; //RST 38h
	}

	//A:00 F:11 B:22 C:33 D:44 E:55 H:66 L:77 SP:8888 PC:9999 PCMEM:AA,BB,CC,DD
}

void CPU::ProcessCBInstruction()
{
	//SRL B
	//debug = true;
	/*getchar();
	printf("Process CB instruction!\n");*/

	uint8_t opcode = this->emulator->memory->Read(this->PC);
	this->PC++;

	switch (opcode)
	{
	case 0x00: this->opcode_cb_rlc(this->BC.msb); break; //RLC B
	case 0x01: this->opcode_cb_rlc(this->BC.lsb); break; //RLC C
	case 0x02: this->opcode_cb_rlc(this->DE.msb); break; //RLC D
	case 0x03: this->opcode_cb_rlc(this->DE.lsb); break; //RLC E
	case 0x04: this->opcode_cb_rlc(this->HL.msb); break; //RLC H
	case 0x05: this->opcode_cb_rlc(this->HL.lsb); break; //RLC L
	case 0x06: this->opcode_cb_rlc(this->HL); break; //RLC (HL)
	case 0x07: this->opcode_cb_rlc(this->AF.msb); break; //RLC A
	case 0x08: this->opcode_cb_rrc(this->BC.msb); break; //RRC B
	case 0x09: this->opcode_cb_rrc(this->BC.lsb); break; //RRC C
	case 0x0A: this->opcode_cb_rrc(this->DE.msb); break; //RRC D
	case 0x0B: this->opcode_cb_rrc(this->DE.lsb); break; //RRC E
	case 0x0C: this->opcode_cb_rrc(this->HL.msb); break; //RRC H
	case 0x0D: this->opcode_cb_rrc(this->HL.lsb); break; //RRC L
	case 0x0E: this->opcode_cb_rrc(this->HL); break; //RRC (HL)
	case 0x0F: this->opcode_cb_rrc(this->AF.msb); break; //RRC A
	case 0x10: this->opcode_cb_rl(this->BC.msb); break; //RL B
	case 0x11: this->opcode_cb_rl(this->BC.lsb); break; //RL C
	case 0x12: this->opcode_cb_rl(this->DE.msb); break; //RL D
	case 0x13: this->opcode_cb_rl(this->DE.lsb); break; //RL E
	case 0x14: this->opcode_cb_rl(this->HL.msb); break; //RL H
	case 0x15: this->opcode_cb_rl(this->HL.lsb); break; //RL L
	case 0x16: this->opcode_cb_rl(this->HL); break; //RL (HL)
	case 0x17: this->opcode_cb_rl(this->AF.msb); break; //RL A
	case 0x18: this->opcode_cb_rr(this->BC.msb); break; //RR B
	case 0x19: this->opcode_cb_rr(this->BC.lsb); break; //RR C
	case 0x1A: this->opcode_cb_rr(this->DE.msb); break; //RR D
	case 0x1B: this->opcode_cb_rr(this->DE.lsb); break; //RR E
	case 0x1C: this->opcode_cb_rr(this->HL.msb); break; //RR H
	case 0x1D: this->opcode_cb_rr(this->HL.lsb); break; //RR L
	case 0x1E: this->opcode_cb_rr(this->HL); break; //RR (HL)
	case 0x1F: this->opcode_cb_rr(this->AF.msb); break; //RR A
	case 0x20: this->opcode_cb_sla(this->BC.msb); break; //SLA B
	case 0x21: this->opcode_cb_sla(this->BC.lsb); break; //SLA C
	case 0x22: this->opcode_cb_sla(this->DE.msb); break; //SLA D
	case 0x23: this->opcode_cb_sla(this->DE.lsb); break; //SLA E
	case 0x24: this->opcode_cb_sla(this->HL.msb); break; //SLA H
	case 0x25: this->opcode_cb_sla(this->HL.lsb); break; //SLA L
	case 0x26: this->opcode_cb_sla(this->HL); break; //SLA (HL)
	case 0x27: this->opcode_cb_sla(this->AF.msb); break; //SLA A
	case 0x28: this->opcode_cb_sra(this->BC.msb); break; //SRA B
	case 0x29: this->opcode_cb_sra(this->BC.lsb); break; //SRA C
	case 0x2A: this->opcode_cb_sra(this->DE.msb); break; //SRA D
	case 0x2B: this->opcode_cb_sra(this->DE.lsb); break; //SRA E
	case 0x2C: this->opcode_cb_sra(this->HL.msb); break; //SRA H
	case 0x2D: this->opcode_cb_sra(this->HL.lsb); break; //SRA L
	case 0x2E: this->opcode_cb_sra(this->HL); break; //SRA (HL)
	case 0x2F: this->opcode_cb_sra(this->AF.msb); break; //SRA A
	case 0x30: this->opcode_cb_swap(this->BC.msb); break; //SWAP B
	case 0x31: this->opcode_cb_swap(this->BC.lsb); break; //SWAP C
	case 0x32: this->opcode_cb_swap(this->DE.msb); break; //SWAP D
	case 0x33: this->opcode_cb_swap(this->DE.lsb); break; //SWAP E
	case 0x34: this->opcode_cb_swap(this->HL.msb); break; //SWAP H
	case 0x35: this->opcode_cb_swap(this->HL.lsb); break; //SWAP L
	case 0x36: this->opcode_cb_swap(this->HL); break; //SWAP (HL)
	case 0x37: this->opcode_cb_swap(this->AF.msb); break; //SWAP A
	case 0x38: this->opcode_cb_srl(this->BC.msb); break; //SRL B
	case 0x39: this->opcode_cb_srl(this->BC.lsb); break; //SRL C
	case 0x3A: this->opcode_cb_srl(this->DE.msb); break; //SRL D
	case 0x3B: this->opcode_cb_srl(this->DE.lsb); break; //SRL E
	case 0x3C: this->opcode_cb_srl(this->HL.msb); break; //SRL H
	case 0x3D: this->opcode_cb_srl(this->HL.lsb); break; //SRL L
	case 0x3E: this->opcode_cb_srl(this->HL); break; //SRL (HL)
	case 0x3F: this->opcode_cb_srl(this->AF.msb); break; //SRL A
	case 0x40: this->opcode_cb_bit(0, this->BC.msb); break; //BIT 0, B
	case 0x41: this->opcode_cb_bit(0, this->BC.lsb); break; //BIT 0, C
	case 0x42: this->opcode_cb_bit(0, this->DE.msb); break; //BIT 0, D
	case 0x43: this->opcode_cb_bit(0, this->DE.lsb); break; //BIT 0, E
	case 0x44: this->opcode_cb_bit(0, this->HL.msb); break; //BIT 0, H
	case 0x45: this->opcode_cb_bit(0, this->HL.lsb); break; //BIT 0, L
	case 0x46: this->opcode_cb_bit(0, this->HL); break; //BIT 0, (HL)
	case 0x47: this->opcode_cb_bit(0, this->AF.msb); break; //BIT 0, A
	case 0x48: this->opcode_cb_bit(1, this->BC.msb); break; //BIT 1, B
	case 0x49: this->opcode_cb_bit(1, this->BC.lsb); break; //BIT 1, C
	case 0x4A: this->opcode_cb_bit(1, this->DE.msb); break; //BIT 1, D
	case 0x4B: this->opcode_cb_bit(1, this->DE.lsb); break; //BIT 1, E
	case 0x4C: this->opcode_cb_bit(1, this->HL.msb); break; //BIT 1, H
	case 0x4D: this->opcode_cb_bit(1, this->HL.lsb); break; //BIT 1, L
	case 0x4E: this->opcode_cb_bit(1, this->HL); break; //BIT 1, (HL)
	case 0x4F: this->opcode_cb_bit(1, this->AF.msb); break; //BIT 1, A
	case 0x50: this->opcode_cb_bit(2, this->BC.msb); break; //BIT 2, B
	case 0x51: this->opcode_cb_bit(2, this->BC.lsb); break; //BIT 2, C
	case 0x52: this->opcode_cb_bit(2, this->DE.msb); break; //BIT 2, D
	case 0x53: this->opcode_cb_bit(2, this->DE.lsb); break; //BIT 2, E
	case 0x54: this->opcode_cb_bit(2, this->HL.msb); break; //BIT 2, H
	case 0x55: this->opcode_cb_bit(2, this->HL.lsb); break; //BIT 2, L
	case 0x56: this->opcode_cb_bit(2, this->HL); break; //BIT 2, (HL)
	case 0x57: this->opcode_cb_bit(2, this->AF.msb); break; //BIT 2, A
	case 0x58: this->opcode_cb_bit(3, this->BC.msb); break; //BIT 3, B
	case 0x59: this->opcode_cb_bit(3, this->BC.lsb); break; //BIT 3, C
	case 0x5A: this->opcode_cb_bit(3, this->DE.msb); break; //BIT 3, D
	case 0x5B: this->opcode_cb_bit(3, this->DE.lsb); break; //BIT 3, E
	case 0x5C: this->opcode_cb_bit(3, this->HL.msb); break; //BIT 3, H
	case 0x5D: this->opcode_cb_bit(3, this->HL.lsb); break; //BIT 3, L
	case 0x5E: this->opcode_cb_bit(3, this->HL); break; //BIT 3, (HL)
	case 0x5F: this->opcode_cb_bit(3, this->AF.msb); break; //BIT 3, A
	case 0x60: this->opcode_cb_bit(4, this->BC.msb); break; //BIT 4, B
	case 0x61: this->opcode_cb_bit(4, this->BC.lsb); break; //BIT 4, C
	case 0x62: this->opcode_cb_bit(4, this->DE.msb); break; //BIT 4, D
	case 0x63: this->opcode_cb_bit(4, this->DE.lsb); break; //BIT 4, E
	case 0x64: this->opcode_cb_bit(4, this->HL.msb); break; //BIT 4, H
	case 0x65: this->opcode_cb_bit(4, this->HL.lsb); break; //BIT 4, L
	case 0x66: this->opcode_cb_bit(4, this->HL); break; //BIT 4, (HL)
	case 0x67: this->opcode_cb_bit(4, this->AF.msb); break; //BIT 4, A
	case 0x68: this->opcode_cb_bit(5, this->BC.msb); break; //BIT 5, B
	case 0x69: this->opcode_cb_bit(5, this->BC.lsb); break; //BIT 5, C
	case 0x6A: this->opcode_cb_bit(5, this->DE.msb); break; //BIT 5, D
	case 0x6B: this->opcode_cb_bit(5, this->DE.lsb); break; //BIT 5, E
	case 0x6C: this->opcode_cb_bit(5, this->HL.msb); break; //BIT 5, H
	case 0x6D: this->opcode_cb_bit(5, this->HL.lsb); break; //BIT 5, L
	case 0x6E: this->opcode_cb_bit(5, this->HL); break; //BIT 5, (HL)
	case 0x6F: this->opcode_cb_bit(5, this->AF.msb); break; //BIT 5, A
	case 0x70: this->opcode_cb_bit(6, this->BC.msb); break; //BIT 6, B
	case 0x71: this->opcode_cb_bit(6, this->BC.lsb); break; //BIT 6, C
	case 0x72: this->opcode_cb_bit(6, this->DE.msb); break; //BIT 6, D
	case 0x73: this->opcode_cb_bit(6, this->DE.lsb); break; //BIT 6, E
	case 0x74: this->opcode_cb_bit(6, this->HL.msb); break; //BIT 6, H
	case 0x75: this->opcode_cb_bit(6, this->HL.lsb); break; //BIT 6, L
	case 0x76: this->opcode_cb_bit(6, this->HL); break; //BIT 6, (HL)
	case 0x77: this->opcode_cb_bit(6, this->AF.msb); break; //BIT 6, A
	case 0x78: this->opcode_cb_bit(7, this->BC.msb); break; //BIT 7, B
	case 0x79: this->opcode_cb_bit(7, this->BC.lsb); break; //BIT 7, C
	case 0x7A: this->opcode_cb_bit(7, this->DE.msb); break; //BIT 7, D
	case 0x7B: this->opcode_cb_bit(7, this->DE.lsb); break; //BIT 7, E
	case 0x7C: this->opcode_cb_bit(7, this->HL.msb); break; //BIT 7, H
	case 0x7D: this->opcode_cb_bit(7, this->HL.lsb); break; //BIT 7, L
	case 0x7E: this->opcode_cb_bit(7, this->HL); break; //BIT 7, (HL)
	case 0x7F: this->opcode_cb_bit(7, this->AF.msb); break; //BIT 7, A
	case 0x80: this->opcode_cb_res(0, this->BC.msb); break; //RES 0, B
	case 0x81: this->opcode_cb_res(0, this->BC.lsb); break; //RES 0, C
	case 0x82: this->opcode_cb_res(0, this->DE.msb); break; //RES 0, D
	case 0x83: this->opcode_cb_res(0, this->DE.lsb); break; //RES 0, E
	case 0x84: this->opcode_cb_res(0, this->HL.msb); break; //RES 0, H
	case 0x85: this->opcode_cb_res(0, this->HL.lsb); break; //RES 0, L
	case 0x86: this->opcode_cb_res(0, this->HL); break; //RES 0, (HL)
	case 0x87: this->opcode_cb_res(0, this->AF.msb); break; //RES 0, A
	case 0x88: this->opcode_cb_res(1, this->BC.msb); break; //RES 1, B
	case 0x89: this->opcode_cb_res(1, this->BC.lsb); break; //RES 1, C
	case 0x8A: this->opcode_cb_res(1, this->DE.msb); break; //RES 1, D
	case 0x8B: this->opcode_cb_res(1, this->DE.lsb); break; //RES 1, E
	case 0x8C: this->opcode_cb_res(1, this->HL.msb); break; //RES 1, H
	case 0x8D: this->opcode_cb_res(1, this->HL.lsb); break; //RES 1, L
	case 0x8E: this->opcode_cb_res(1, this->HL); break; //RES 1, (HL)
	case 0x8F: this->opcode_cb_res(1, this->AF.msb); break; //RES 1, A
	case 0x90: this->opcode_cb_res(2, this->BC.msb); break; //RES 2, B
	case 0x91: this->opcode_cb_res(2, this->BC.lsb); break; //RES 2, C
	case 0x92: this->opcode_cb_res(2, this->DE.msb); break; //RES 2, D
	case 0x93: this->opcode_cb_res(2, this->DE.lsb); break; //RES 2, E
	case 0x94: this->opcode_cb_res(2, this->HL.msb); break; //RES 2, H
	case 0x95: this->opcode_cb_res(2, this->HL.lsb); break; //RES 2, L
	case 0x96: this->opcode_cb_res(2, this->HL); break; //RES 2, (HL)
	case 0x97: this->opcode_cb_res(2, this->AF.msb); break; //RES 2, A
	case 0x98: this->opcode_cb_res(3, this->BC.msb); break; //RES 3, B
	case 0x99: this->opcode_cb_res(3, this->BC.lsb); break; //RES 3, C
	case 0x9A: this->opcode_cb_res(3, this->DE.msb); break; //RES 3, D
	case 0x9B: this->opcode_cb_res(3, this->DE.lsb); break; //RES 3, E
	case 0x9C: this->opcode_cb_res(3, this->HL.msb); break; //RES 3, H
	case 0x9D: this->opcode_cb_res(3, this->HL.lsb); break; //RES 3, L
	case 0x9E: this->opcode_cb_res(3, this->HL); break; //RES 3, (HL)
	case 0x9F: this->opcode_cb_res(3, this->AF.msb); break; //RES 3, A
	case 0xA0: this->opcode_cb_res(4, this->BC.msb); break; //RES 4, B
	case 0xA1: this->opcode_cb_res(4, this->BC.lsb); break; //RES 4, C
	case 0xA2: this->opcode_cb_res(4, this->DE.msb); break; //RES 4, D
	case 0xA3: this->opcode_cb_res(4, this->DE.lsb); break; //RES 4, E
	case 0xA4: this->opcode_cb_res(4, this->HL.msb); break; //RES 4, H
	case 0xA5: this->opcode_cb_res(4, this->HL.lsb); break; //RES 4, L
	case 0xA6: this->opcode_cb_res(4, this->HL); break; //RES 4, (HL)
	case 0xA7: this->opcode_cb_res(4, this->AF.msb); break; //RES 4, A
	case 0xA8: this->opcode_cb_res(5, this->BC.msb); break; //RES 5, B
	case 0xA9: this->opcode_cb_res(5, this->BC.lsb); break; //RES 5, C
	case 0xAA: this->opcode_cb_res(5, this->DE.msb); break; //RES 5, D
	case 0xAB: this->opcode_cb_res(5, this->DE.lsb); break; //RES 5, E
	case 0xAC: this->opcode_cb_res(5, this->HL.msb); break; //RES 5, H
	case 0xAD: this->opcode_cb_res(5, this->HL.lsb); break; //RES 5, L
	case 0xAE: this->opcode_cb_res(5, this->HL); break; //RES 5, (HL)
	case 0xAF: this->opcode_cb_res(5, this->AF.msb); break; //RES 5, A
	case 0xB0: this->opcode_cb_res(6, this->BC.msb); break; //RES 6, B
	case 0xB1: this->opcode_cb_res(6, this->BC.lsb); break; //RES 6, C
	case 0xB2: this->opcode_cb_res(6, this->DE.msb); break; //RES 6, D
	case 0xB3: this->opcode_cb_res(6, this->DE.lsb); break; //RES 6, E
	case 0xB4: this->opcode_cb_res(6, this->HL.msb); break; //RES 6, H
	case 0xB5: this->opcode_cb_res(6, this->HL.lsb); break; //RES 6, L
	case 0xB6: this->opcode_cb_res(6, this->HL); break; //RES 6, (HL)
	case 0xB7: this->opcode_cb_res(6, this->AF.msb); break; //RES 6, A
	case 0xB8: this->opcode_cb_res(7, this->BC.msb); break; //RES 7, B
	case 0xB9: this->opcode_cb_res(7, this->BC.lsb); break; //RES 7, C
	case 0xBA: this->opcode_cb_res(7, this->DE.msb); break; //RES 7, D
	case 0xBB: this->opcode_cb_res(7, this->DE.lsb); break; //RES 7, E
	case 0xBC: this->opcode_cb_res(7, this->HL.msb); break; //RES 7, H
	case 0xBD: this->opcode_cb_res(7, this->HL.lsb); break; //RES 7, L
	case 0xBE: this->opcode_cb_res(7, this->HL); break; //RES 7, (HL)
	case 0xBF: this->opcode_cb_res(7, this->AF.msb); break; //RES 7, A
	case 0xC0: this->opcode_cb_set(0, this->BC.msb); break; //SET 0, B
	case 0xC1: this->opcode_cb_set(0, this->BC.lsb); break; //SET 0, C
	case 0xC2: this->opcode_cb_set(0, this->DE.msb); break; //SET 0, D
	case 0xC3: this->opcode_cb_set(0, this->DE.lsb); break; //SET 0, E
	case 0xC4: this->opcode_cb_set(0, this->HL.msb); break; //SET 0, H
	case 0xC5: this->opcode_cb_set(0, this->HL.lsb); break; //SET 0, L
	case 0xC6: this->opcode_cb_set(0, this->HL); break; //SET 0, (HL)
	case 0xC7: this->opcode_cb_set(0, this->AF.msb); break; //SET 0, A
	case 0xC8: this->opcode_cb_set(1, this->BC.msb); break; //SET 1, B
	case 0xC9: this->opcode_cb_set(1, this->BC.lsb); break; //SET 1, C
	case 0xCA: this->opcode_cb_set(1, this->DE.msb); break; //SET 1, D
	case 0xCB: this->opcode_cb_set(1, this->DE.lsb); break; //SET 1, E
	case 0xCC: this->opcode_cb_set(1, this->HL.msb); break; //SET 1, H
	case 0xCD: this->opcode_cb_set(1, this->HL.lsb); break; //SET 1, L
	case 0xCE: this->opcode_cb_set(1, this->HL); break; //SET 1, (HL)
	case 0xCF: this->opcode_cb_set(1, this->AF.msb); break; //SET 1, A
	case 0xD0: this->opcode_cb_set(2, this->BC.msb); break; //SET 2, B
	case 0xD1: this->opcode_cb_set(2, this->BC.lsb); break; //SET 2, C
	case 0xD2: this->opcode_cb_set(2, this->DE.msb); break; //SET 2, D
	case 0xD3: this->opcode_cb_set(2, this->DE.lsb); break; //SET 2, E
	case 0xD4: this->opcode_cb_set(2, this->HL.msb); break; //SET 2, H
	case 0xD5: this->opcode_cb_set(2, this->HL.lsb); break; //SET 2, L
	case 0xD6: this->opcode_cb_set(2, this->HL); break; //SET 2, (HL)
	case 0xD7: this->opcode_cb_set(2, this->AF.msb); break; //SET 2, A
	case 0xD8: this->opcode_cb_set(3, this->BC.msb); break; //SET 3, B
	case 0xD9: this->opcode_cb_set(3, this->BC.lsb); break; //SET 3, C
	case 0xDA: this->opcode_cb_set(3, this->DE.msb); break; //SET 3, D
	case 0xDB: this->opcode_cb_set(3, this->DE.lsb); break; //SET 3, E
	case 0xDC: this->opcode_cb_set(3, this->HL.msb); break; //SET 3, H
	case 0xDD: this->opcode_cb_set(3, this->HL.lsb); break; //SET 3, L
	case 0xDE: this->opcode_cb_set(3, this->HL); break; //SET 3, (HL)
	case 0xDF: this->opcode_cb_set(3, this->AF.msb); break; //SET 3, A
	case 0xE0: this->opcode_cb_set(4, this->BC.msb); break; //SET 4, B
	case 0xE1: this->opcode_cb_set(4, this->BC.lsb); break; //SET 4, C
	case 0xE2: this->opcode_cb_set(4, this->DE.msb); break; //SET 4, D
	case 0xE3: this->opcode_cb_set(4, this->DE.lsb); break; //SET 4, E
	case 0xE4: this->opcode_cb_set(4, this->HL.msb); break; //SET 4, H
	case 0xE5: this->opcode_cb_set(4, this->HL.lsb); break; //SET 4, L
	case 0xE6: this->opcode_cb_set(4, this->HL); break; //SET 4, (HL)
	case 0xE7: this->opcode_cb_set(4, this->AF.msb); break; //SET 4, A
	case 0xE8: this->opcode_cb_set(5, this->BC.msb); break; //SET 5, B
	case 0xE9: this->opcode_cb_set(5, this->BC.lsb); break; //SET 5, C
	case 0xEA: this->opcode_cb_set(5, this->DE.msb); break; //SET 5, D
	case 0xEB: this->opcode_cb_set(5, this->DE.lsb); break; //SET 5, E
	case 0xEC: this->opcode_cb_set(5, this->HL.msb); break; //SET 5, H
	case 0xED: this->opcode_cb_set(5, this->HL.lsb); break; //SET 5, L
	case 0xEE: this->opcode_cb_set(5, this->HL); break; //SET 5, (HL)
	case 0xEF: this->opcode_cb_set(5, this->AF.msb); break; //SET 5, A
	case 0xF0: this->opcode_cb_set(6, this->BC.msb); break; //SET 6, B
	case 0xF1: this->opcode_cb_set(6, this->BC.lsb); break; //SET 6, C
	case 0xF2: this->opcode_cb_set(6, this->DE.msb); break; //SET 6, D
	case 0xF3: this->opcode_cb_set(6, this->DE.lsb); break; //SET 6, E
	case 0xF4: this->opcode_cb_set(6, this->HL.msb); break; //SET 6, H
	case 0xF5: this->opcode_cb_set(6, this->HL.lsb); break; //SET 6, L
	case 0xF6: this->opcode_cb_set(6, this->HL); break; //SET 6, (HL)
	case 0xF7: this->opcode_cb_set(6, this->AF.msb); break; //SET 6, A
	case 0xF8: this->opcode_cb_set(7, this->BC.msb); break; //SET 7, B
	case 0xF9: this->opcode_cb_set(7, this->BC.lsb); break; //SET 7, C
	case 0xFA: this->opcode_cb_set(7, this->DE.msb); break; //SET 7, D
	case 0xFB: this->opcode_cb_set(7, this->DE.lsb); break; //SET 7, E
	case 0xFC: this->opcode_cb_set(7, this->HL.msb); break; //SET 7, H
	case 0xFD: this->opcode_cb_set(7, this->HL.lsb); break; //SET 7, L
	case 0xFE: this->opcode_cb_set(7, this->HL); break; //SET 7, (HL)
	case 0xFF: this->opcode_cb_set(7, this->AF.msb); break; //SET 7, A
	}
}
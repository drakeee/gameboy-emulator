#include <Main.h>

using namespace GameBoy;

CPU::CPU(Memory* memory) :
	memory(memory)
{

}

CPU::~CPU()
{

}

void CPU::ProcessInstruction()
{
	uint8_t opcode = this->memory->Read(this->PC);
	this->PC++;

	switch (opcode)
	{
	case 0x00:
		break;
	}
}
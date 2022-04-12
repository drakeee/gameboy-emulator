#include <Main.h>

using namespace GameBoy;

Memory::Memory(Cartridge* cartridge)
{
	FILE* file = cartridge->GetFile();
	fseek(file, 0, SEEK_SET);
	fread(this->memory, 1, cartridge->GetFileSize(), file);
}

Memory::Memory(uint8_t data[0xFFFF])
{
	memcpy(this->memory, data, 0xFFFF);
}

Memory::~Memory()
{

}

void Memory::Write(uint16_t address, uint8_t value)
{
	this->memory[address] = value;
}

uint8_t Memory::Read(uint16_t address)
{
	return this->memory[address];
}
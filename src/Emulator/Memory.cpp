#include <Main.h>

using namespace GameBoy;

Memory::Memory(Cartridge* cartridge)
{
	FILE* file = cartridge->GetFile();
	fseek(file, 0, SEEK_SET);
	fread(&this->m_Memory, 1, sizeof(this->m_Memory), file);

	this->m_Rom = new uint8_t[cartridge->GetFileSize()];
	fseek(file, 0, SEEK_SET);
	fread(this->m_Rom, 1, cartridge->GetFileSize(), file);

	printf("Memory: %02X\n", this->m_Memory[0]);
	printf("Memory: %02X\n", this->m_Memory[1]);

	printf("ROM: %02X\n", this->m_Rom[0]);
	printf("ROM: %02X\n", this->m_Rom[1]);
}

Memory::Memory(uint8_t data[0xFFFF])
{
	memcpy(this->m_Memory, data, 0xFFFF);
}

Memory::~Memory()
{

}

void Memory::Write(uint16_t address, uint8_t value)
{
	this->m_Memory[address] = value;
}

uint8_t Memory::Read(uint16_t address)
{
	return this->m_Memory[address];
}
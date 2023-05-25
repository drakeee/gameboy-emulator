#include <Main.h>

using namespace GameBoy;

Memory::Memory(Cartridge* cartridge)
{
	FILE* file = cartridge->GetFile();
	fseek(file, 0, SEEK_SET);

	fseek(file, 0, SEEK_SET);
	fread(this->m_Memory, 1, sizeof(this->m_Memory), file);

	this->m_Rom = new uint8_t[cartridge->GetFileSize()];
	this->m_RomSize = cartridge->GetFileSize();

	fseek(file, 0, SEEK_SET);
	fread(this->m_Rom, 1, cartridge->GetFileSize(), file);

	printf("Cycle: %d\n", GameBoy::CPU::opcode_cycles.at(this->m_Memory[0]));

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
	if (address == 0x0000)
	{
		printf("HUHHH!\n");
	}

	if (address == 0xFF01)
	{
		char c = (char)value;
		printf("%c", c);
		this->m_Memory[0xFF02] = 0;
	}

	if (address == 0xDEFE)
	{
		//printf("DEBUG ADDRESS!\n");
		//getchar();
	}

	this->m_Memory[address] = value;
}

uint8_t Memory::Read(uint16_t address)
{
	/*if (address == 0xFF44)
		return 0x90;*/

	/*if (0xFF00 <= address <= 0xFF4B)
		return ReadIO(address);*/

	/*if(address >= 0xFF00)
		printf("Memory: %04X - %02X\n", address, this->m_Memory[address]);*/

	return this->m_Memory[address];
}

uint8_t Memory::ReadIO(uint16_t address)
{
	switch (address)
	{
	case 0xFF44:
		return 0;
	}
}
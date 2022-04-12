#include <Main.h>

using namespace GameBoy;

Emulator::Emulator(const std::string& filePath) :
	Emulator(filePath.c_str())
{

}

Emulator::Emulator(const char* filePath)
{
	if (FILE* file = fopen(filePath, "r"))
	{
		printf("Error: Unable to open path \"%s\"\n", filePath);
		fclose(file);
		return;
	}

	this->cartridge = new Cartridge(filePath);
	this->memory = new Memory(this->cartridge);
	this->graphics = new Graphics();
	this->cpu = new CPU(this->memory);

	this->memory->Read();
}

Emulator::~Emulator()
{

}
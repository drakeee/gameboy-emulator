#include <Main.h>

#include <fstream>

using namespace GameBoy;

Emulator::Emulator(const std::string& filePath) :
	Emulator(filePath.c_str())
{

}

Emulator::Emulator(const char* filePath)
{
	std::ifstream f(filePath);
	bool fileExists = f.good();
	f.close();

	if (!fileExists)
	{
		printf("Error: Unable to open path \"%s\"\n", filePath);
		return;
	}

	this->cartridge = new Cartridge(filePath);
	this->memory = new Memory(this->cartridge);
	this->graphics = new Graphics();
	this->cpu = new CPU(this->memory);
}

Emulator::~Emulator()
{

}
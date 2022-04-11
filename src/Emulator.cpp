#include <Main.h>

using namespace GameBoy;

Emulator::Emulator()
{
	this->memory = new Memory();
	this->cpu = new CPU(this->memory);
	this->cartridge = new Cartridge("");
	this->graphics = new Graphics();
}

Emulator::~Emulator()
{

}
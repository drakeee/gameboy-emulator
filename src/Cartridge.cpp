#include <Main.h>

using namespace GameBoy;
Cartridge::Cartridge(const char* filePath)
{
	this->cartridge = fopen(filePath, "rb");
	if (this->cartridge == nullptr)
	{
		printf("File not found: \"%s\"", filePath);
		return;
	}

	fseek(this->cartridge, 0, SEEK_END);
	this->cartridgeFileSize = ftell(this->cartridge);
	fseek(this->cartridge, 0, SEEK_SET);

	this->ReadHeader();
}

Cartridge::~Cartridge()
{

}

void Cartridge::ReadHeader()
{
	if (this->cartridgeRead)
		return;

	memset(&this->header, 0, sizeof(this->header));

	fseek(this->cartridge, 0x0100, SEEK_SET);
	fread(&this->header, 1, sizeof(this->header), this->cartridge);
	fseek(this->cartridge, 0, SEEK_SET);

	printf("%s\n", this->header.title);
	printf("%02X\n", this->header.cgbFlag);
	printf("%02X\n", this->header.sgbFlag);
	printf("%02X\n", this->header.cartridgeType);
	printf("%02X\n", this->header.destinationCode);
}
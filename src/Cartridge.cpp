#include <Main.h>

#include <map>
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace GameBoy;
Cartridge::Cartridge(const char* filePath)
{
	this->cartridge = fopen(filePath, "rb");
	if (this->cartridge == nullptr)
	{
		printf("File not found: \"%s\"\n", filePath);
		return;
	}

	this->filePath.assign(filePath);

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

	fseek(this->cartridge, 0x014B, SEEK_SET);
	uint8_t someShit = 0;
	fread(&someShit, 1, sizeof(someShit), this->cartridge);
	printf("someShit: %d\n", someShit);

	fseek(this->cartridge, 0x0100, SEEK_SET);
	fread(&this->header, 1, sizeof(this->header), this->cartridge);
	fseek(this->cartridge, 0, SEEK_SET);

	printf("%s\n", this->header.title);
	printf("%02X\n", this->header.cgbFlag);
	printf("%02X\n", this->header.sgbFlag);
	printf("%02X\n", this->header.cartridgeType);
	printf("%02X\n", this->header.destinationCode);
}

FILE* Cartridge::GetFile(void)
{
	return this->cartridge;
}

uint32_t Cartridge::GetFileSize(void)
{
	return this->cartridgeFileSize;
}

std::string Cartridge::GetFilePath(void)
{
#ifdef _WIN32
	char fullFilename[MAX_PATH];
	GetFullPathName(this->filePath.c_str(), MAX_PATH, fullFilename, nullptr);

	return std::string(fullFilename);
#else
	return "";
#endif
}

std::string Cartridge::GetFileName(void)
{
#ifdef _WIN32
	int index = this->filePath.find_last_of("/\\");
#else
	int index = this->filePath.find_last_of("/");
#endif

	return this->filePath.substr(index + 1);
}

std::string Cartridge::GetType(void)
{
	switch (this->header.cartridgeType)
	{
		case 0x00: return "ROM ONLY";
		case 0x01: return "MBC1";
		case 0x02: return "MBC1+RAM";
		case 0x03: return "MBC1+RAM+BATTERY";
		case 0x05: return "MBC2";
		case 0x06: return "MBC2+BATTERY";
		case 0x08: return "ROM+RAM";
		case 0x09: return "ROM+RAM+BATTERY";
		case 0x0B: return "MMM01";
		case 0x0C: return "MMM01+RAM";
		case 0x0D: return "MMM01+RAM+BATTERY";
		case 0x0F: return "MBC3+TIMER+BATTERY";
		case 0x10: return "MBC3+TIMER+RAM+BATTERY";
		case 0x11: return "MBC3";
		case 0x12: return "MBC3+RAM";
		case 0x13: return "MBC3+RAM+BATTERY";
		case 0x19: return "MBC5";
		case 0x1A: return "MBC5+RAM";
		case 0x1B: return "MBC5+RAM+BATTERY";
		case 0x1C: return "MBC5+RUMBLE";
		case 0x1D: return "MBC5+RUMBLE+RAM";
		case 0x1E: return "MBC5+RUMBLE+RAM+BATTERY";
		case 0x20: return "MBC6";
		case 0x22: return "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
		case 0xFC: return "POCKET CAMERA";
		case 0xFD: return "BANDAI TAMA5";
		case 0xFE: return "HuC3";
		case 0xFF: return "HuC1+RAM+BATTERY";
		default: return "UNKOWN";
	}
}

std::string Cartridge::GetRAMSize(void)
{
	switch (header.ramSize)
	{
		case 0x00: return "None";
		case 0x01: return "2 KBytes";
		case 0x02: return "8 Kbytes";
		case 0x03: return "32 KBytes (4 banks of 8KBytes each)";
		case 0x04: return "128 KBytes (16 banks of 8KBytes each";
		case 0x05: return "64 KBytes (8 banks of 8KBytes each)";
		default: return "Unknown";
	}
}

std::string Cartridge::GetROMSize(void)
{
	switch (header.romSize)
	{
	case 0x00: return "32KByte (no ROM banking)";
		case 0x01: return "64KByte (4 banks)";
		case 0x02: return "128KByte (8 banks)";
		case 0x03: return "256KByte (16 banks)";
		case 0x04: return "512KByte (32 banks)";
		case 0x05: return "1MByte (64 banks)";
		case 0x06: return "2MByte (128 banks)";
		case 0x07: return "4MByte (256 banks)";
		case 0x52: return "1.1MByte (72 banks)";
		case 0x53: return "1.2MByte (80 banks)";
		case 0x54: return "1.5MByte (96 banks)";
	}
}

std::string Cartridge::GetOldLicensee(void)
{
	switch (header.oldLicenseeCode)
	{
		case 0x00: return "None";
		case 0x01: return "Nintendo";
		case 0x08: return "Capcom";
		case 0x09: return "Hot-b";
		case 0x0A: return "Jaleco";
		case 0x0B: return "Coconuts";
		case 0x0C: return "Elite Systems";
		case 0x13: return "Electronic Arts";
		case 0x18: return "Hudsonsoft";
		case 0x19: return "ITC Entertainment";
		case 0x1A: return "Yanoman";
		case 0x1D: return "Clary";
		case 0x1F: return "Virgin";
		case 0x24: return "PCM Complete";
		case 0x25: return "San-x";
		case 0x28: return "Kotobuki Systems";
		case 0x29: return "Seta";
		case 0x30: return "Infogrames";
		case 0x31: return "Nintendo";
		case 0x32: return "Bandai";
		case 0x33: return "GameBoy Color - See New Licensee";
		case 0x34: return "Konami";
		case 0x35: return "Hector";
		case 0x38: return "Capcom";
		case 0x39: return "Banpresto";
		case 0x3C: return "*Entertainment i";
		case 0x3E: return "Gremlin";
		case 0x41: return "Ubi Soft";
		case 0x42: return "Atlus";
		case 0x44: return "Malibu";
		case 0x46: return "Angel";
		case 0x47: return "Spectrum Holoby";
		case 0x49: return "Irem";
		case 0x4A: return "Virgin";
		case 0x4D: return "Malibu";
		case 0x4F: return "U.S. Gold";
		case 0x50: return "Absolute";
		case 0x51: return "Acclaim";
		case 0x52: return "Activision";
		case 0x53: return "American Sammy";
		case 0x54: return "Gametek";
		case 0x55: return "Park Place";
		case 0x56: return "LJN";
		case 0x57: return "Matchbox";
		case 0x59: return "Milton Bradley";
		case 0x5A: return "Mindscape";
		case 0x5B: return "Romstar";
		case 0x5C: return "Naxat Soft";
		case 0x5D: return "Tradewest";
		case 0x60: return "Titus";
		case 0x61: return "Virgin";
		case 0x67: return "Ocean";
		case 0x69: return "Electronic Arts";
		case 0x6E: return "Elite Systems";
		case 0x6F: return "Electro Brain";
		case 0x70: return "Infogames";
		case 0x71: return "Interplay";
		case 0x72: return "Broderbund";
		case 0x73: return "Sculptered Soft";
		case 0x75: return "The Sales Curve";
		case 0x78: return "T*HQ";
		case 0x79: return "Accolade";
		case 0x7A: return "Triffix Entertainment";
		case 0x7C: return "Microprose";
		case 0x7F: return "Kemco";
		case 0x80: return "Misawa Entertainment";
		case 0x83: return "Lozc";
		case 0x86: return "Tokuma Shoten Intermedia";
		case 0x8B: return "Bullet-proof Software";
		case 0x8C: return "Vic Tokai";
		case 0x8E: return "Ape";
		case 0x8F: return "I'max";
		case 0x91: return "Chun Soft";
		case 0x92: return "Video System";
		case 0x93: return "Tsuburava";
		case 0x95: return "Varie";
		case 0x96: return "Yonezawa/S'pal";
		case 0x97: return "Kaneko";
		case 0x99: return "Arc";
		case 0x9A: return "Nihon Bussan";
		case 0x9B: return "Tecmo";
		case 0x9C: return "Imagineer";
		case 0x9D: return "Banpresto";
		case 0x9F: return "Nova";
		case 0xA1: return "Hori Electric";
		case 0xA2: return "Bandai";
		case 0xA4: return "Konami";
		case 0xA6: return "Kawada";
		case 0xA7: return "Takara";
		case 0xA9: return "Technos Japan";
		case 0xAA: return "Broderbund";
		case 0xAC: return "Toei Animation";
		case 0xAD: return "Toho";
		case 0xAF: return "Namco";
		case 0xB0: return "Acclaim";
		case 0xB1: return "Ascii or Nexoft";
		case 0xB2: return "Bandai";
		case 0xB4: return "Enix";
		case 0xB6: return "HAL";
		case 0xB7: return "SNK";
		case 0xB9: return "Pony Canyon";
		case 0xBA: return "*Culture Brain O";
		case 0xBB: return "Sunsoft";
		case 0xBD: return "Sony Imagesoft";
		case 0xBF: return "Sammy";
		case 0xC0: return "Taito";
		case 0xC2: return "Kemco";
		case 0xC3: return "Squaresoft";
		case 0xC4: return "Tokuma Shoten Intermedia";
		case 0xC5: return "Data East";
		case 0xC6: return "Tonkin House";
		case 0xC8: return "Koei";
		case 0xC9: return "UFL";
		case 0xCA: return "Ultra";
		case 0xCB: return "Vap";
		case 0xCC: return "USE";
		case 0xCD: return "Meldac";
		case 0xCE: return "*Pony Canyon or";
		case 0xCF: return "Angel";
		case 0xD0: return "Taito";
		case 0xD1: return "Sofel";
		case 0xD2: return "Quest";
		case 0xD3: return "Sigma Enterprises";
		case 0xD4: return "Ask Kodansha";
		case 0xD6: return "Naxat Soft";
		case 0xD7: return "Copya Systems";
		case 0xD9: return "Banpresto";
		case 0xDA: return "Tomy";
		case 0xDB: return "LJN";
		case 0xDD: return "NCS";
		case 0xDE: return "Human";
		case 0xDF: return "Altron";
		case 0xE0: return "Jaleco";
		case 0xE1: return "Towachiki";
		case 0xE2: return "Uutaka";
		case 0xE3: return "Varie";
		case 0xE5: return "Epoch";
		case 0xE7: return "Athena";
		case 0xE8: return "Asmik";
		case 0xE9: return "Natsume";
		case 0xEA: return "King Records";
		case 0xEB: return "Atlus";
		case 0xEC: return "Epic/Sony Records";
		case 0xEE: return "IGS";
		case 0xF0: return "A Wave";
		case 0xF3: return "Extreme Entertainment";
		case 0xFF: return "LJN";
		default: return "Unkown";
	}
}

const std::map<std::string, std::string> licenseeCodes = {
		{"01", "Nintendo"},
		{"02", "Rocket Games"},
		{"08", "Capcom"},
		{"09", "Hot B Co."},
		{"0A", "Jaleco"},
		{"0B", "Coconuts Japan"},
		{"0C", "Coconuts Japan/G.X.Media"},
		{"0H", "Starfish"},
		{"0L", "Warashi Inc."},
		{"0N", "Nowpro"},
		{"0P", "Game Village"},
		{"13", "Electronic Arts Japan"},
		{"18", "Hudson Soft Japan"},
		{"19", "S.C.P."},
		{"1A", "Yonoman"},
		{"1G", "SMDE"},
		{"1P", "Creatures Inc."},
		{"1Q", "TDK Deep Impresion"},
		{"20", "Destination Software"},
		{"22", "VR 1 Japan"},
		{"25", "San-X"},
		{"28", "Kemco Japan"},
		{"29", "Seta"},
		{"2H", "Ubisoft Japan"},
		{"2K", "NEC InterChannel"},
		{"2L", "Tam"},
		{"2M", "Jordan"},
		{"2N", "Smilesoft"},
		{"2Q", "Mediakite"},
		{"36", "Codemasters"},
		{"37", "GAGA Communications"},
		{"38", "Laguna"},
		{"39", "Telstar Fun and Games"},
		{"41", "Ubi Soft Entertainment"},
		{"42", "Sunsoft"},
		{"47", "Spectrum Holobyte"},
		{"49", "IREM"},
		{"4D", "Malibu Games"},
		{"4F", "Eidos/U.S. Gold"},
		{"4J", "Fox Interactive"},
		{"4K", "Time Warner Interactive"},
		{"4Q", "Disney"},
		{"4S", "Black Pearl"},
		{"4X", "GT Interactive"},
		{"4Y", "RARE"},
		{"4Z", "Crave Entertainment"},
		{"50", "Absolute Entertainment"},
		{"51", "Acclaim"},
		{"52", "Activision"},
		{"53", "American Sammy Corp."},
		{"54", "Take 2 Interactive"},
		{"55", "Hi Tech"},
		{"56", "LJN LTD."},
		{"58", "Mattel"},
		{"5A", "Mindscape/Red Orb Ent."},
		{"5C", "Taxan"},
		{"5D", "Midway"},
		{"5F", "American Softworks"},
		{"5G", "Majesco Sales Inc"},
		{"5H", "3DO"},
		{"5K", "Hasbro"},
		{"5L", "NewKidCo"},
		{"5M", "Telegames"},
		{"5N", "Metro3D"},
		{"5P", "Vatical Entertainment"},
		{"5Q", "LEGO Media"},
		{"5S", "Xicat Interactive"},
		{"5T", "Cryo Interactive"},
		{"5W", "Red Storm Ent./BKN Ent."},
		{"5X", "Microids"},
		{"5Z", "Conspiracy Entertainment Corp."},
		{"60", "Titus Interactive Studios"},
		{"61", "Virgin Interactive"},
		{"62", "Maxis"},
		{"64", "LucasArts Entertainment"},
		{"67", "Ocean"},
		{"69", "Electronic Arts"},
		{"6E", "Elite Systems Ltd."},
		{"6F", "Electro Brain"},
		{"6G", "The Learning Company"},
		{"6H", "BBC"},
		{"6J", "Software 2000"},
		{"6L", "BAM! Entertainment"},
		{"6M", "Studio 3"},
		{"6Q", "Classified Games"},
		{"6S", "TDK Mediactive"},
		{"6U", "DreamCatcher"},
		{"6V", "JoWood Productions"},
		{"6W", "SEGA"},
		{"6X", "Wannado Edition"},
		{"6Y", "LSP"},
		{"6Z", "ITE Media"},
		{"70", "Infogrames"},
		{"71", "Interplay"},
		{"72", "JVC Musical Industries Inc."},
		{"73", "Parker Brothers"},
		{"75", "SCI"},
		{"78", "THQ"},
		{"79", "Accolade"},
		{"7A", "Triffix Ent. Inc."},
		{"7C", "Microprose Software"},
		{"7D", "Universal Interactive Studios"},
		{"7F", "Kemco"},
		{"7G", "Rage Software"},
		{"7H", "Encore"},
		{"7J", "Zoo"},
		{"7K", "BVM"},
		{"7L", "Simon & Schuster Interactive"},
		{"7M", "Asmik Ace Entertainment Inc./AIA"},
		{"7N", "Empire Interactive"},
		{"7Q", "Jester Interactive"},
		{"7T", "Scholastic"},
		{"7U", "Ignition Entertainment"},
		{"7W", "Stadlbauer"},
		{"80", "Misawa"},
		{"83", "LOZC"},
		{"8B", "Bulletproof Software"},
		{"8C", "Vic Tokai Inc."},
		{"8J", "General Entertainment"},
		{"8N", "Success"},
		{"8P", "SEGA Japan"},
		{"91", "Chun Soft"},
		{"92", "Video System"},
		{"93", "BEC"},
		{"96", "Yonezawa/S'pal"},
		{"97", "Kaneko"},
		{"99", "Victor Interactive Software"},
		{"9A", "Nichibutsu/Nihon Bussan"},
		{"9B", "Tecmo"},
		{"9C", "Imagineer"},
		{"9F", "Nova"},
		{"9H", "Bottom Up"},
		{"9L", "Hasbro Japan"},
		{"9N", "Marvelous Entertainment"},
		{"9P", "Keynet Inc."},
		{"9Q", "Hands-On Entertainment"},
		{"A0", "Telenet"},
		{"A1", "Hori"},
		{"A4", "Konami"},
		{"A6", "Kawada"},
		{"A7", "Takara"},
		{"A9", "Technos Japan Corp."},
		{"AA", "JVC"},
		{"AC", "Toei Animation"},
		{"AD", "Toho"},
		{"AF", "Namco"},
		{"AG", "Media Rings Corporation"},
		{"AH", "J-Wing"},
		{"AK", "KID"},
		{"AL", "MediaFactory"},
		{"AP", "Infogrames Hudson"},
		{"AQ", "Kiratto. Ludic Inc"},
		{"B0", "Acclaim Japan"},
		{"B1", "ASCII"},
		{"B2", "Bandai"},
		{"B4", "Enix"},
		{"B6", "HAL Laboratory"},
		{"B7", "SNK"},
		{"B9", "Pony Canyon Hanbai"},
		{"BA", "Culture Brain"},
		{"BB", "Sunsoft"},
		{"BD", "Sony Imagesoft"},
		{"BF", "Sammy"},
		{"BG", "Magical"},
		{"BJ", "Compile"},
		{"BL", "MTO Inc."},
		{"BN", "Sunrise Interactive"},
		{"BP", "Global A Entertainment"},
		{"BQ", "Fuuki"},
		{"C0", "Taito"},
		{"C2", "Kemco"},
		{"C3", "Square Soft"},
		{"C5", "Data East"},
		{"C6", "Tonkin House"},
		{"C8", "Koei"},
		{"CA", "Konami/Palcom/Ultra"},
		{"CB", "Vapinc/NTVIC"},
		{"CC", "Use Co.,Ltd."},
		{"CD", "Meldac"},
		{"CE", "FCI/Pony Canyon"},
		{"CF", "Angel"},
		{"CM", "Konami Computer Entertainment Osaka"},
		{"CP", "Enterbrain"},
		{"D1", "Sofel"},
		{"D2", "Quest"},
		{"D3", "Sigma Enterprises"},
		{"D4", "Ask Kodansa"},
		{"D6", "Naxat"},
		{"D7", "Copya System"},
		{"D9", "Banpresto"},
		{"DA", "TOMY"},
		{"DB", "LJN Japan"},
		{"DD", "NCS"},
		{"DF", "Altron Corporation"},
		{"DH", "Gaps Inc."},
		{"DN", "ELF"},
		{"E2", "Yutaka"},
		{"E3", "Varie"},
		{"E5", "Epoch"},
		{"E7", "Athena"},
		{"E8", "Asmik Ace Entertainment Inc."},
		{"E9", "Natsume"},
		{"EA", "King Records"},
		{"EB", "Atlus"},
		{"EC", "Epic/Sony Records"},
		{"EE", "IGS"},
		{"EL", "Spike"},
		{"EM", "Konami Computer Entertainment Tokyo"},
		{"EN", "Alphadream Corporation"},
		{"F0", "A Wave"},
		{"G1", "PCCW"},
		{"G4", "KiKi Co Ltd"},
		{"G5", "Open Sesame Inc."},
		{"G6", "Sims"},
		{"G7", "Broccoli"},
		{"G8", "Avex"},
		{"G9", "D3 Publisher"},
		{"GB", "Konami Computer Entertainment Japan"},
		{"GD", "Square-Enix"},
		{"HY", "Sachen"}
};

std::string Cartridge::GetNewLicensee(void)
{
	std::string licensee{ (char)header.newLicenseeCode[0], (char)header.newLicenseeCode[1] };
	
	std::map<std::string, std::string>::const_iterator a = licenseeCodes.find(licensee);
	if (a != licenseeCodes.end())
		return a->second;

	return "Unkown";
}
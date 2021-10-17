#pragma once

#include <Main.h>

namespace GameBoy
{
	enum LicenseeCodes
	{
		None = 0x00,
		NINTENDORD1 = 0x01,
		CAPCOM = 0x08,
		EA1 = 0x13,
		HUDSON_SOFT = 0x18,
		BAI = 0x19,
		KSS = 0x20,
		POW = 0x22,
		PCM_COMPLETE = 0x24,
		SANX = 0x25,
		KEMCO_JAPAN = 0x28,
		SETA = 0x29,
		VIACOM = 0x30,
		NINTENDO = 0x31,
		BANDAI = 0x32,
		OCEAN_ACCLAIM1 = 0x33,
		KONAMI1 = 0x34,
		HECTOR = 0x35,
		TAITO = 0x37,
		HUDSON = 0x38,
		BANPRESTO = 0x39,
		UBISOFT = 0x41,
		ATLUS = 0x42,
		MALIBU = 0x44,
		ANGEL = 0x46,
		BULLET_PROOF = 0x47,
		IREM = 0x49,
		ABSOLUTE = 0x50,
		ACCLAIM = 0x51,
		ACTIVISION = 0x52,
		AMERICAN_SAMMY = 0x53,
		KONAMI2 = 0x54,
		HI_TECH_ENTERTAINMENT = 0x55,
		LJN = 0x56,
		MATCHBOX = 0x57,
		MATTEL = 0x58,
		MILTON_BRADLEY = 0x59,
		TITUS = 0x60,
		VIRGIN = 0x61,
		LUCASARTS = 0x64,
		OCEAN = 0x67,
		EA2 = 0x69,
		INFOGRAMES = 0x70,
		INTERPLAY = 0x71,
		BRODERBUND = 0x72,
		SCULPTURED = 0x73,
		SCI = 0x75,
		THQ = 0x78,
		ACCOLADE = 0x79,
		MISAWA = 0x80,
		LOZC = 0x83,
		TOKUMA_SHOTEN_INTERMEDIA = 0x86,
		TSUKUDA_ORIGINAL = 0x87,
		CHUNSOFT = 0x91,
		VIDEO_SYSTEM = 0x92,
		OCEAN_ACCLAIM2 = 0x93,
		VARIE = 0x95,
		YONEZAWA = 0x96,
		KANEKO = 0x97,
		PACK_IN_SOFT = 0x99,
		YUGIOH = 0xA4
	};

	struct CartridgeHeader
	{
		uint8_t entryPoint[4];
		uint8_t nintendoLogo[48];
		uint8_t title[15];
		uint8_t cgbFlag;
		uint8_t newLicenseeCode[2];
		uint8_t sgbFlag;
		uint8_t cartridgeType;
		uint8_t romSize;
		uint8_t ramSize;
		uint8_t destinationCode;
		uint8_t oldLicenseeCode;
		uint8_t romVersionNumber;
		uint8_t headerChecksum;
		uint16_t globalChecksum;
	};

	class Cartridge
	{
	public:
		Cartridge(const char* filePath);
		~Cartridge();

	private:
		void ReadHeader(void);

		FILE* cartridge = nullptr;
		CartridgeHeader header;

		uint32_t cartridgeFileSize = 0;
		bool cartridgeRead = false;
	};
}
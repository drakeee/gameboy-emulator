#include <Emulator/Graphics.h>

#include <Core/Application.h>

#include <bitset>

using namespace GameBoy;

Graphics::Graphics(Emulator* emulator) :
    m_Emulator(emulator)
{
	for (size_t i = 0; i < 128; i++)
	{
		//Tiles[i] = Tile();
	}
}

void Graphics::OnRender()
{
	if (this->m_Emulator->memory == nullptr)
		return;

	uint8_t* memory = this->m_Emulator->memory->GetMemory();
	if (memory == nullptr)
		return;

	uint16_t baseIndex = Tile::TileSetOne;
	uint16_t endIndex = 0x87FF;

	uint8_t tileData[16];
	for (uint16_t i = baseIndex; i <= endIndex; i += 16)
	{
		uint16_t tileIndex = (i - baseIndex) / 16;

		memcpy_s(tileData, 16, &memory[i], 16);

		this->Tiles[tileIndex] = tileData;

		//Switch to second tiles set
		if ((i + 16) == endIndex + 1)
		{
			i = 0x8800 - 1;
			endIndex = 0x8FFF;
		}
	}
}

Graphics::Tile::Tile()
{
	GUI::Application& app = GUI::Application::Get();

	SDL_Window* window = app.GetWindow();
	SDL_Renderer* renderer = app.GetRenderer();

	if (window == nullptr)
		printf("WINDOW NULLPTR!\n");

	if (renderer == nullptr)
		printf("RENDERER NULLPTR!\n");

	this->m_Texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 8, 8);
}

Graphics::Tile::Tile(uint8_t tileData[16]) :
	Tile()
{
    memcpy_s(this->m_TileData, 16, tileData, 16);

	RenderTile();
}

Graphics::Tile::~Tile()
{
	if(m_Texture != nullptr)
		SDL_DestroyTexture(m_Texture);
}

void Graphics::Tile::RenderTile()
{
	if (this->m_Texture == nullptr)
		printf("RENDER NULLPTR!\n");

	if (m_TileData[0] == '\0')
		return;

	uint8_t* pixels;
	int pitch;

	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if (format == nullptr)
		printf("FORMAT FAILED!\n");

	int lockSuccess = SDL_LockTexture(this->m_Texture, NULL, (void**)&pixels, &pitch);
	if (lockSuccess != 0)
		printf("LockTexture wasn't successfull");

	for (uint16_t y = 0; y < 8; y++)
	{
		uint8_t highByte = m_TileData[y * 2 + 1];
		uint8_t lowByte = m_TileData[y * 2 + 0];

		for (uint16_t x = 0; x < 8; x++)
		{
			bool highBit = CHECK_BIT(highByte, BIT(7 - x));
			bool lowBit = CHECK_BIT(lowByte, BIT(7 - x));

			static std::bitset<8> colorId;
			colorId.reset();
			colorId.set(0, lowBit);
			colorId.set(1, highBit);

			uint32_t color = (uint8_t)colorId.to_ulong() + 1;
			color = 256 / color - 64;
			uint32_t tileColor = SDL_MapRGBA(format, color, color, color, 255);

			int index = x * 4 + y * pitch;
			uint32_t* p = (uint32_t*)&pixels[index];

			*p = tileColor;
		}

	}

	SDL_UnlockTexture(this->m_Texture);
	SDL_FreeFormat(format);
}
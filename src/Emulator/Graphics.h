#pragma once

#include <Main.h>

struct SDL_Texture;
namespace GameBoy
{
	class Graphics
	{
	public:
		Graphics(Emulator* emulator);

		class Tile
		{
		public:
			static const uint16_t TileSetZero = 0x8000;
			static const uint16_t TileSetOne = 0x8800;

			Tile();
			Tile(uint8_t tileData[16]);
			~Tile();

			void* GetTexture() { return (void*)m_Texture; }

			Tile& operator=(uint8_t data[16])
			{
				memcpy_s(this->m_TileData, 16, data, 16);
				
				RenderTile();

				return *this; // return the reference to LHS object.
			}

		private:
			SDL_Texture* m_Texture = nullptr;

			uint8_t m_TileData[16] = { '\0' };

			void RenderTile();
		};

		Tile Tiles[256];
		void OnRender();

	private:
		Emulator* m_Emulator = nullptr;
	};
}
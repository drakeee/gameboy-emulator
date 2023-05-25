#include <Core/Layers/GameTiles.h>

#include <imgui/imgui_internal.h>

#include <array>
#include <bitset>

using namespace GUI;
void GameTilesLayer::OnAttach()
{
	SDL_Window* window = this->m_App->GetWindow();
	SDL_Renderer* renderer = this->m_App->GetRenderer();

	this->m_Texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 8, 8);
}

uint8_t testTiles[] = {
	//0x3C, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x5E, 0x7E, 0x0A, 0x7C, 0x56, 0x38, 0x7C
	//0xFF, 0x00, 0x7E, 0xFF, 0x85, 0x81, 0x89, 0x83, 0x93, 0x85, 0xA5, 0x8B, 0xC9, 0x97, 0x7E, 0xFF
	0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD, 0xCD
};

void GameTilesLayer::RenderTestTile()
{
	uint8_t* pixels;
	int pitch;

	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	SDL_LockTexture(this->m_Texture, NULL, (void**)&pixels, &pitch);

	uint32_t WHITE_COLOR = SDL_MapRGBA(format, 255, 255, 255, 255);
	uint32_t BLACK_COLOR = SDL_MapRGBA(format, 0, 0, 0, 255);

	for (uint16_t y = 0; y < 8; y++)
	{
		uint8_t highByte = testTiles[y * 2 + 1];
		uint8_t lowByte = testTiles[y * 2 + 0];

		for (uint16_t x = 0; x < 8; x++)
		{
			bool highBit = CHECK_BIT(highByte, BIT(7 - x));
			bool lowBit = CHECK_BIT(lowByte, BIT(7 - x));

			std::bitset<8> colorId;
			colorId.set(0, lowBit);
			colorId.set(1, highBit);

			uint32_t color = (uint8_t)colorId.to_ulong() + 1;
			color = 256/color - 64;
			uint32_t tileColor = SDL_MapRGBA(format, color, color, color, 255);

			int index = x * 4 + y * pitch;
			uint32_t* p = (uint32_t*)&pixels[index];

			*p = tileColor;
		}

		//printf("%02X %02X\n", highByte, lowByte);
	}

	SDL_UnlockTexture(this->m_Texture);
	SDL_FreeFormat(format);
}

//void Focus(int row_index)
//{
//	ImGuiContext& g = *ImGui::GetCurrentContext();
//	ImGuiWindow* window = g.CurrentWindow;
//	// Item height is hard coded here
//	int item_height = 17;
//	window->Scroll.y = IM_FLOOR(item_height * row_index);
//}

void GameTilesLayer::OnImGuiRender()
{
	ImGui::Begin("Tiles");

	if (this->m_App->currentEmulator)
	{
		for (uint16_t i = 0; i < 256; i++)
		{
			ImVec2 bremaining = ImGui::GetContentRegionAvail();
			if (bremaining.x < 32)
				ImGui::NewLine();

			GameBoy::Graphics::Tile& t = this->m_App->currentEmulator->graphics->Tiles[i];
			ImGui::Image(t.GetTexture(), ImVec2(32, 32));
			ImGui::SameLine();
		}

		/*GameBoy::Graphics::Tile& t = this->m_App->currentEmulator->graphics->Tiles[0];
		t = testTiles;

		ImGui::Image(t.GetTexture(), ImVec2(32, 32));

		for (uint16_t tileIndex = 1; tileIndex < 128; tileIndex++)
		{
			GameBoy::Graphics::Tile& t = this->m_App->currentEmulator->graphics->Tiles[tileIndex];

			uint8_t* memory = this->m_App->currentEmulator->memory->GetMemory();
			uint8_t tileData[16];
			memcpy_s(tileData, 16, &memory[0x8800 + tileIndex * 16], 16);

			t = tileData;

			ImGui::Image(t.GetTexture(), ImVec2(32, 32));
			ImGui::SameLine();
			ImGui::Text("%04X", 0x8000 + tileIndex * 16);
		}*/


		//if (ImGui::BeginTable("AssemblyTable", 3, ImGuiTableFlags_Borders))
		//{
		//	ImGui::TableSetupColumn("ROM");
		//	ImGui::TableSetupColumn("Values");
		//	ImGui::TableSetupColumn("Assembly");
		//	ImGui::TableHeadersRow();

		//	uint8_t* memory = this->m_App->currentEmulator->memory->GetMemory();
  //          //uint32_t memorySize = this->m_App->currentEmulator->memory->GetROMSize();
		//	uint32_t memorySize = 0xFFFF;

		//	for (uint32_t i = 0; i < memorySize;)
		//	{
		//		if (i > memorySize)
		//			break;

  //              uint8_t opcode = memory[i];

		//		//ImGui::TableNextRow();

		//		//uint16_t currentPC = this->m_App->currentEmulator->cpu->PC;

		//		//ImGui::TableNextColumn();
		//		//ImGui::Text("%04X", i);

		//		//ImGui::TableNextColumn();

		//		//if (opcode < 256)
		//		//{
		//		//	uint8_t cycle = GameBoy::CPU::opcode_lengths.at(opcode);
		//		//	for(uint8_t c = 0; c < cycle; c++)
		//		//		ImGui::Text("%02X", memory[i + c]), ImGui::SameLine();
		//		//}

		//		//ImGui::TableNextColumn();
		//		//ImGui::Text("%s", GameBoy::CPU::opcode_names.at(opcode).c_str());
  //  //            
		//		//if (currentPC == i)
		//		//{
		//		//	/*ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 1)));
		//		//	ImGui::ScrollToItem(ImGuiScrollFlags_KeepVisibleCenterY);*/
		//		//}

  //  //            if (opcode < 256)
  //  //            {
  //  //                //printf("opcode: %02X - %d - %d\n", opcode, opcode, opcode_cycles.at(opcode));
		//		//	uint8_t cycle = GameBoy::CPU::opcode_lengths.at(opcode);
		//		//	if (cycle == 0)
		//		//		cycle = 1;

  //  //                i += cycle;
		//		//}
		//		//else
		//		i++;
  //              /*if (opcode < 256)
  //                  i += opcode_cycles.at(i) - 1;*/
		//	}

		////	/*ImGui::TableNextRow();

		////	ImGui::TableNextColumn();
		////	ImGui::Text("Model");
		////	ImGui::TableNextColumn();
		////	ImGui::Text("Test");

		////	ImGui::TableNextRow();

		////	ImGui::TableNextColumn();
		////	ImGui::Text("Model");
		////	ImGui::TableNextColumn();
		////	ImGui::Text("Test");

		////	ImGui::TableNextRow();

		////	ImGui::TableNextColumn();
		////	ImGui::Text("Model");
		////	ImGui::TableNextColumn();
		////	ImGui::Text("Test");

		////	ImGui::TableNextRow();

		////	ImGui::TableNextColumn();
		////	ImGui::Text("Model");
		////	ImGui::TableNextColumn();
		////	ImGui::Text("Test");

		////	ImGui::TableNextRow();

		////	ImGui::TableNextColumn();
		////	ImGui::Text("Model");
		////	ImGui::TableNextColumn();
		////	ImGui::Text("Test");

		////	ImGui::TableNextRow();

		////	ImGui::TableNextColumn();
		////	ImGui::Text("Model");
		////	ImGui::TableNextColumn();
		////	ImGui::Text("Test");

		////	ImGui::TableNextRow();

		////	ImGui::TableNextColumn();
		////	ImGui::Text("Model");
		////	ImGui::TableNextColumn();
		////	ImGui::Text("Test");*/

		//	ImGui::EndTable();
		//}
	}

	ImGui::End();
}

void GameTilesLayer::OnKeyDown(const uint8_t* keys)
{

}
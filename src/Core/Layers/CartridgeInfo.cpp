#include <Core/Layers/CartridgeInfo.h>
#include <Emulator/Emulator.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace GUI;

void CartridgeInfoLayer::OnAttach()
{
	SDL_Window* window = this->m_App->GetWindow();
	SDL_Renderer* renderer = this->m_App->GetRenderer();

	this->m_Texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, this->m_NintendoWidth, this->m_NintendoHeight);
}

void CartridgeInfoLayer::OnImGuiRender()
{
	this->RenderNintendoLogo();

	ImGui::Begin("Cartridge Info", false);

	GameBoy::Emulator* emulator = this->m_App->currentEmulator;
	if (emulator)
	{
		GameBoy::Cartridge* cartridge = this->m_App->currentEmulator->cartridge;
		if (cartridge == nullptr)
		{
			ImGui::End();
			return;
		}

		GameBoy::CartridgeHeader& header = cartridge->GetHeader();
		ImGui::Text("Current tab: %s", header.title);

		ImGui::Text("File: %s", cartridge->GetFileName().c_str());
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", cartridge->GetFilePath().c_str());

		ImGui::NewLine();
		ImGui::Text("Title: %s", header.title);
		ImGui::Text("CGB Flags: %02X", header.cgbFlag);
		ImGui::Text("SGB Flags: %s", (header.sgbFlag == 0x03 ? "Game supports SGB functions" : "No SGB supports"));
		ImGui::Text("Cartridge Type: %s", cartridge->GetType().c_str());
		ImGui::Text("ROM Size: %s", cartridge->GetROMSize().c_str());
		ImGui::Text("RAM Size: %s", cartridge->GetRAMSize().c_str());
		ImGui::Text("Destination Code: %s", (header.destinationCode ? "Non-Japanese" : "Japanese"));

		if (header.oldLicenseeCode == 0x33)
			ImGui::Text("Licensee Code: %s", cartridge->GetNewLicensee().c_str());
		else
			ImGui::Text("Licensee Code: %s", cartridge->GetOldLicensee().c_str());

		ImGui::NewLine();
		ImGui::Text("Nintendo Logo Check:");
		ImGui::Image((void*)this->m_Texture, ImVec2(192, 32));
	}

	ImGui::End();
}

void CartridgeInfoLayer::OnKeyDown(const uint8_t* keys)
{

}

#define LOW_NIBBLE(byte) (byte & 0x0F) // write low quartet
#define HIGH_NIBBLE(byte) ((byte & 0xF0) >> 4) // write high quartet

void CartridgeInfoLayer::RenderNintendoLogo()
{
	if (Application::Get().currentEmulator == nullptr)
		return;

	GameBoy::Cartridge* cartridge = Application::Get().currentEmulator->cartridge;
	if (cartridge->error)
		return;

	if (this->m_Texture == nullptr)
		return;

	uint8_t* pixels;
	int pitch;

	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	SDL_LockTexture(this->m_Texture, NULL, (void**)&pixels, &pitch);

	uint32_t WHITE_COLOR = SDL_MapRGBA(format, 255, 255, 255, 255);
	uint32_t BLACK_COLOR = SDL_MapRGBA(format, 0, 0, 0, 255);

	for (size_t y = 0; y < this->m_NintendoHeight; y++)
	{
		int idx = ((y / 2) % 2) + (y / 4) * 24;
		for (size_t x = 0; x < this->m_NintendoWidth; x += 4, idx += 2)
		{
			int index = x * 4 + y * pitch;
			uint32_t* p = (uint32_t*)&pixels[index];
			uint8_t pi = cartridge->GetHeader().nintendoLogo[idx];

			uint8_t nibble = !(y % 2) ? HIGH_NIBBLE(pi) : LOW_NIBBLE(pi);
			for (short i = 0; i < 4; i++)
			{
				*p = nibble & BIT(3 - i) ? BLACK_COLOR : WHITE_COLOR;
				p += 1;
			}
		}
	}

	SDL_UnlockTexture(this->m_Texture);
	SDL_FreeFormat(format);
}
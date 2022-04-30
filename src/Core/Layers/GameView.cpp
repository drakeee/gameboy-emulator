#include <Emulator/Emulator.h>
#include <Core/Layers/GameView.h>

#include <imgui/imgui_memory_editor.h>

#include <string>

using namespace GUI;
void GameViewLayer::OnAttach()
{

}

void GameViewLayer::OnImGuiRender()
{
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
	ImGui::SetNextWindowCollapsed(false);
	ImGui::SetNextWindowClass(&window_class);
	ImGui::Begin("Games", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

	if (ImGui::BeginTabBar("#games"))
	{
		int index = 0;
		for (GameBoy::Emulator* emulator : Application::Get().GetAllEmulators())
		{
			std::string title{ (char*)emulator->cartridge->GetHeader().title };
			title.append(std::string() + "##" + std::to_string(index));

			bool currentEmulator = (Application::Get().currentEmulator == emulator);
			if (ImGui::BeginTabItem(title.c_str()))
			{
				if (this->m_App->currentEmulator != emulator)
				{
					this->m_App->currentEmulator = emulator;
					printf("Switched to %s\n", title.c_str());
				}

				ImGui::EndTabItem();
			}

			index++;
		}
		
		/*GameBoy::Emulator* emulator = this->m_App->currentEmulator;
		if (emulator)
		{
			GameBoy::Cartridge* cartridge = emulator->cartridge;
			if (!cartridge->error)
			{

			}
		}*/

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void GameViewLayer::OnKeyDown(const uint8_t* keys)
{

}
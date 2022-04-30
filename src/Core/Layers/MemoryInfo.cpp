#include <Emulator/Emulator.h>
#include <Core/Layers/MemoryInfo.h>

#include <imgui/imgui_memory_editor.h>

#include <string>

using namespace GUI;
void MemoryInfoLayer::OnAttach()
{

}

void MemoryInfoLayer::OnImGuiRender()
{
	static MemoryEditor mem_edit_2;
	ImGui::Begin("Memory View");

	if (ImGui::BeginTabBar("#games"))
	{
		if (ImGui::BeginTabItem("Memory"))
		{
			if (this->m_App->currentEmulator)
			{
				mem_edit_2.DrawContents(this->m_App->currentEmulator->memory->GetMemory(), 0xFFFF);
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Full ROM"))
		{
			if (this->m_App->currentEmulator)
			{
				GameBoy::Emulator* emulator = this->m_App->currentEmulator;
				mem_edit_2.DrawContents(emulator->memory->GetFullROM(), 0xFFFF);
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void MemoryInfoLayer::OnKeyDown(const uint8_t* keys)
{

}
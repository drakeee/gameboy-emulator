#include <Core/Layers/RegistersInfo.h>

using namespace GUI;
void RegistersInfoLayer::OnAttach()
{

}

char buffer[32];
void RegistersInfoLayer::OnImGuiRender()
{
	ImGui::Begin("Registers Info");

	if (this->m_App->currentEmulator)
	{
		GameBoy::CPU* cpu = this->m_App->currentEmulator->cpu;

		static ImVec2 textWidth = ImGui::CalcTextSize("FFFFF");
		ImGui::PushItemWidth(textWidth.x);

		ImGui::Text("A:");
		{
			ImGui::SameLine();
			sprintf_s(buffer, sizeof(buffer), "%04X", cpu->AF.GetValue());
			ImGui::InputText("##a", buffer, 32, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsHexadecimal);
		}

		ImGui::Text("Flags: ");
		{
			bool carry = CHECK_BIT(cpu->AF.GetLSB(), GameBoy::CPU::CARRY);
			ImGui::Checkbox("C", &carry);

			ImGui::SameLine();

			bool halfCarry = CHECK_BIT(cpu->AF.GetLSB(), GameBoy::CPU::HALF_CARRY);
			ImGui::Checkbox("HC", &halfCarry);

			ImGui::SameLine();

			bool subtraction = CHECK_BIT(cpu->AF.GetLSB(), GameBoy::CPU::SUBTRACTION);
			ImGui::Checkbox("S", &subtraction);

			ImGui::SameLine();

			bool zero = CHECK_BIT(cpu->AF.GetLSB(), GameBoy::CPU::ZERO);
			ImGui::Checkbox("Z", &zero);
		}

		ImGui::NewLine();

		ImGui::Text("PC:");
		{
			ImGui::SameLine();
			sprintf_s(buffer, sizeof(buffer), "%04X", cpu->PC);
			ImGui::InputText("##pc", buffer, 32, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsHexadecimal);
		}
		ImGui::SameLine();
		ImGui::Text("SP:");
		{
			ImGui::SameLine();
			sprintf_s(buffer, sizeof(buffer), "%04X", cpu->SP);
			ImGui::InputText("##sp", buffer, 32, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsHexadecimal);
		}
		ImGui::Text("BC:");
		{
			ImGui::SameLine();
			sprintf_s(buffer, sizeof(buffer), "%04X", cpu->BC.GetValue());
			ImGui::InputText("##bc", buffer, 32, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsHexadecimal);
		}
		ImGui::SameLine();
		ImGui::Text("DE:");
		{
			ImGui::SameLine();
			sprintf_s(buffer, sizeof(buffer), "%04X", cpu->DE.GetValue());
			ImGui::InputText("##de", buffer, 32, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsHexadecimal);
		}
		ImGui::Text("HL:");
		{
			ImGui::SameLine();
			sprintf_s(buffer, sizeof(buffer), "%04X", cpu->HL.GetValue());
			ImGui::InputText("##hl", buffer, 32, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsHexadecimal);
		}

		uint8_t opcode = this->m_App->currentEmulator->memory->GetMemory()[cpu->PC];
		if (opcode == 0xCB)
		{
			opcode = this->m_App->currentEmulator->memory->GetMemory()[cpu->PC + 1];
			ImGui::Text("Instruction: %s", GameBoy::CPU::opcode_cb_names.at(opcode).c_str());
		}
		else
			ImGui::Text("Instruction: %02X - %s", opcode, GameBoy::CPU::opcode_names.at(opcode).c_str());
		ImGui::PopItemWidth();
	}

	ImGui::End();
}

void RegistersInfoLayer::OnKeyDown(const uint8_t* keys)
{

}
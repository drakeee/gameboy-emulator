#include <Core/Layers/MenuBar.h>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace GUI;
void MenuBarLayer::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 7));
	if (ImGui::BeginMainMenuBar())
	{
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				this->OpenFile();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Close", "Ctrl+X"))
			{
				this->CloseWindow();
			}

			ImGui::EndMenu();
		}

		char buffer[32];
		sprintf_s(buffer, sizeof(buffer), "FPS: %d", (int)ImGui::GetIO().Framerate);

		float win_width = ImGui::GetWindowSize().x;
		float text_width = ImGui::CalcTextSize(buffer).x;

		ImGui::SetCursorPosX(win_width - text_width - 10.0);
		ImGui::Text(buffer);

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar(1);
}

void MenuBarLayer::OnKeyDown(const uint8_t* keys)
{
	if (keys[SDL_SCANCODE_LCTRL] && keys[SDL_SCANCODE_O])
		this->OpenFile();

	if (keys[SDL_SCANCODE_LCTRL] && keys[SDL_SCANCODE_X])
		this->CloseWindow();
}

void MenuBarLayer::OpenFile()
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "GameBoy Files (*.gb)\0*.gb\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	std::string fileNameStr;

	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;

	printf("Open file: %s\n", fileNameStr.c_str());
	Application::Get().LoadROM(fileNameStr);
}

void MenuBarLayer::CloseWindow()
{
	SDL_Event event;
	event.type = SDL_WINDOWEVENT;
	event.window.event = SDL_WINDOWEVENT_CLOSE;

	SDL_PushEvent(&event);
}
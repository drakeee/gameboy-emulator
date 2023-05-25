#include <Core/Application.h>
#include <Core/Layers/MenuBar.h>
#include <Core/Layers/CartridgeInfo.h>
#include <Core/Layers/GameView.h>
#include <Core/Layers/MemoryInfo.h>
#include <Core/Layers/RegistersInfo.h>
#include <Core/Layers/GameTiles.h>

#include <imgui/imgui_internal.h>

#include <fstream>

using namespace GUI;
Application::Application()
{
	this->m_Window = SDL_CreateWindow(this->m_TitleName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, NULL);
	if (this->m_Window == nullptr)
	{
		printf("Unable to create window: %s\n", SDL_GetError());
		return;
	}

	this->m_Renderer = SDL_CreateRenderer(this->m_Window, -1, SDL_RENDERER_ACCELERATED);
	if (this->m_Renderer == nullptr)
	{
		printf("Unable to create renderer: %s\n", SDL_GetError());
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImFont* robotoFont = io.Fonts->AddFontFromFileTTF("fonts\\Roboto-Medium.ttf", 16);
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL2_InitForSDLRenderer(this->m_Window, this->m_Renderer);
	ImGui_ImplSDLRenderer_Init(this->m_Renderer);

	this->SetStyle();

	//Pushing layers
	this->PushLayer(new MenuBarLayer(this));
	this->PushLayer(new CartridgeInfoLayer(this));
	this->PushLayer(new GameViewLayer(this));
	this->PushLayer(new MemoryInfoLayer(this));
	this->PushLayer(new RegistersInfoLayer(this));
	this->PushLayer(new GameTilesLayer(this));

	for (Layer* layer : this->m_LayerStack)
		layer->OnAttach();

	m_EmulatorThread = std::thread(&Application::ProcessEmulator, this);
}

void Application::ProcessEmulator()
{
	while (!this->shouldClose)
	{
		//std::lock_guard<std::mutex> lock(m_ThreadMutex);
		
		if (this->currentEmulator == nullptr)
			continue;

		m_ThreadMutex.lock();
		this->currentEmulator->cpu->ProcessInstruction();
		m_ThreadMutex.unlock();

		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

#define IMGUI_COLOR(r,g,b,a) ImVec4(r/255.0f,g/255.0f, b/255.0f, a/255.0f)
void Application::SetStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Tab] = IMGUI_COLOR(60, 102, 145, 150);
	style.Colors[ImGuiCol_TabActive] = IMGUI_COLOR(60, 102, 145, 255);
}

void Application::SetTitle(const char* title)
{
	this->m_TitleName.append(title);

	if (this->m_Window != nullptr)
		SDL_SetWindowTitle(this->m_Window, title);
}

void Application::SetSize(int width, int height)
{
	if (this->m_Window != nullptr)
		SDL_SetWindowSize(this->m_Window, width, height);
}

bool Application::LoadROM(std::string fileName)
{
	std::ifstream f(fileName);
	bool fileExists = f.good();
	f.close();

	if (!fileExists)
	{
		printf("Unable to open file: \"%s\"\n", fileName.c_str());
		return false;
	}

	GameBoy::Emulator* newEmulator = new GameBoy::Emulator(fileName);

	if (this->currentEmulator == nullptr)
		this->currentEmulator = newEmulator;

	this->m_EmulatorStack.push_back(newEmulator);
	return true;
}

uint32_t frameDelay;
uint32_t frameStart;
uint32_t frameTime;

bool initial_layout_done = false;
void Application::OnUpdate()
{
	frameDelay = 1000 / this->m_FPS;
	frameStart = SDL_GetTicks();

	if(this->currentEmulator != nullptr)
		this->currentEmulator->graphics->OnRender();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			return;

		if (event.type == SDL_WINDOWEVENT)
		{
			if(event.window.event == SDL_WINDOWEVENT_CLOSE)
				this->shouldClose = true;
		}

		if (event.type == SDL_KEYDOWN /*|| event.type == SDL_KEYUP*/)
		{
			const uint8_t* keys = SDL_GetKeyboardState(nullptr);
			for (Layer* layer : this->m_LayerStack)
				layer->OnKeyDown(keys);
		}

		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	SDL_SetRenderDrawColor(this->m_Renderer,
		this->backgroundColor.r,
		this->backgroundColor.g,
		this->backgroundColor.b,
		this->backgroundColor.a);
	SDL_RenderClear(this->m_Renderer);

	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiID dock_id = ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_PassthruCentralNode);
	if (!initial_layout_done)
	{
		initial_layout_done = true;

		ImGui::DockBuilderRemoveNode(dock_id); // Clear out existing layout
		ImGuiID dock_main_id = ImGui::DockBuilderAddNode(dock_id, ImGuiDockNodeFlags_PassthruCentralNode); // Add empty node
		ImGui::DockBuilderSetNodeSize(dock_id, ImGui::GetMainViewport()->WorkSize);

		ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Left, 0.25f, NULL, &dock_id);
		ImGuiID dock_id_left_down = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.60f, NULL, &dock_id_left);
		ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_id, ImGuiDir_Right, 0.50f, NULL, &dock_id);
		ImGuiID dock_id_far_right = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Right, 0.30f, NULL, &dock_id_right);
		ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.40f, NULL, &dock_id_right);

		ImGui::DockBuilderDockWindow("Cartridge Info", dock_id_left);
		ImGui::DockBuilderDockWindow("Games", dock_id_right);
		ImGui::DockBuilderDockWindow("Tiles", dock_id_far_right);
		ImGui::DockBuilderDockWindow("Memory View", dock_id_down);
		ImGui::DockBuilderDockWindow("Registers Info", dock_id_left_down);

		ImGui::DockBuilderFinish(dock_id);
		printf("Finish dock builder\n");
	}

	//ImGui::SetNextWindowDockID(dock_id_left);

	for (Layer* layer : this->m_LayerStack)
		layer->OnImGuiRender();

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(this->m_Renderer);

	frameTime = SDL_GetTicks() - frameStart;
	if (frameDelay > frameTime)
	{
		SDL_Delay(frameDelay - frameTime);
	}
}

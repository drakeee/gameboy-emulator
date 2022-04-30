#include "Main.h"

#include <vector>
#include <clocale>
#include <bitset>

#define TEST true

int main(int argc, char** args)
{
#if !TEST
	GUI::Application* app = &GUI::Application::Get();
	app->SetTitle("My Precious GameBoy Emulator");
	app->SetMaxFPS(60);

	app->LoadROM("./Tetris.gb");
	app->LoadROM("./Pokemon - Blue Version (USA, Europe) (SGB Enhanced).gb");

	while (!app->shouldClose)
	{
		app->OnUpdate();
	}
#else
	uint8_t test = 0b11111111;
	uint8_t d = (1 << 0);
	bool a = CHECK_BIT(test, (1 << 0));

	std::bitset<8> t(test);
	printf("%d - 0b%s\n", a, t.to_string().c_str());

#endif

	return 0;
}

/*void renderMenu()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 7));

	if(ImGui::BeginMainMenuBar())
	{
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Close", "Ctrl+X"))
			{
				SDL_Event event;
				event.type = SDL_WINDOWEVENT_CLOSE;
				
				SDL_PushEvent(&event);
				//GLFWwindow* window = glfwGetCurrentContext();
				//glfwSetWindowShouldClose(window, true);
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

void renderCartridge(GameBoy::Cartridge* cartridge)
{
	if (cartridge->error)
		return;

	ImGui::Begin("Cartridge", false, ImGuiWindowFlags_AlwaysAutoResize);

	GameBoy::CartridgeHeader& header = cartridge->GetHeader();
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

	if(header.oldLicenseeCode == 0x33)
		ImGui::Text("Licensee Code: %s", cartridge->GetNewLicensee().c_str());
	else
		ImGui::Text("Licensee Code: %s", cartridge->GetOldLicensee().c_str());

	ImGui::NewLine();
	ImGui::Text("Nintendo Logo Check:");
	ImGui::Image((void*)texture, ImVec2(192, 32));

	ImGui::End();
}*/

/*void renderNintendoLogo(GameBoy::Cartridge* cartridge)
{
	if (cartridge->error)
		return;

	if (texture == nullptr)
		return;

	uint8_t* pixels;
	int pitch;

	SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);

	uint32_t WHITE_COLOR = SDL_MapRGBA(format, 255, 255, 255, 255);
	uint32_t BLACK_COLOR = SDL_MapRGBA(format, 0, 0, 0, 255);

	for (size_t y = 0; y < nintendoHeight; y++)
	{
		int idx = ((y / 2) % 2) + (y / 4) * 24;
		for (size_t x = 0; x < nintendoWidth; x += 4, idx += 2)
		{
			int index = x * 4 + y * pitch;
			uint32_t* p = (uint32_t*)&pixels[index];
			uint8_t pi = cartridge->GetHeader().nintendoLogo[idx];

			uint8_t nibble = !(y % 2) ? HIGH_NIBBLE(pi) : LOW_NIBBLE(pi);
			for (short i = 0; i < 4; i++)
			{
				*p = nibble & (1 << (3 - i)) ? BLACK_COLOR : WHITE_COLOR;
				p += 1;
			}

			//printf("%d ", pixelCount + (nintendoWidth/2 - (x / 2)) - 1);
			//printf("%01X ", nibble);
		}
	}

	SDL_UnlockTexture(texture);
	SDL_FreeFormat(format);
}

void renderGameWindow(GameBoy::Cartridge* cartridge)
{
	if (cartridge->error)
		return;

	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
	ImGui::SetNextWindowCollapsed(false);
	ImGui::SetNextWindowClass(&window_class);
	ImGui::Begin("Games", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
	
	if (ImGui::BeginTabBar("#games"))
	{
		if (ImGui::BeginTabItem((char*)cartridge->GetHeader().title))
		{

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

int main(int argNum, char** args)
{
	std::setlocale(LC_ALL, "hu_HU.utf8");
	GameBoy::Cartridge* cartridge = new GameBoy::Cartridge("./Pokemon - Blue Version (USA, Europe) (SGB Enhanced).gb");

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to initialize the SDL2 library\n");
		return 0;
	}

	window = SDL_CreateWindow("My Precious GameBoy Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFont* robotoFont = io.Fonts->AddFontFromFileTTF("fonts\\Roboto-Medium.ttf", 16);
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
	//io.Fonts->AddFontDefault();

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, nintendoWidth, nintendoHeight);

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	uint32_t frameStart;
	int frameTime;

	SDL_Event event;

	bool running = true;
	while (running)
	{
		frameStart = SDL_GetTicks();

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;

			if (event.type == SDL_WINDOWEVENT_CLOSE)
				running = false;

			ImGui_ImplSDL2_ProcessEvent(&event);
		}

		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		SDL_RenderClear(renderer);

		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_PassthruCentralNode);

		renderMenu();
		renderNintendoLogo(cartridge);
		renderCartridge(cartridge);
		renderGameWindow(cartridge);

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks() - frameStart;

		// This keeps us from displaying more frames than 60/Second
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	return 0;
}*/

/*int main()
{
	std::setlocale(LC_ALL, "hu_HU.utf8");

	GameBoy::Cartridge* cartridge = new GameBoy::Cartridge("./Pokemon - Blue Version (USA, Europe) (SGB Enhanced).gb");

	int glfw = glfwInit();
	if (glfw != 1)
	{
		printf("Couldn't initialise GLFW. Return code: %d\n", glfw);
		return 0;
	}

	glfwSetErrorCallback([](int error_code, const char* description)
	{
			printf("Error happened. Code: %d\n", error_code);
			printf("Message: %s\n", description);
	});

	GLFWwindow* window = glfwCreateWindow(800, 640, "My Precious GameBoy Emulator", nullptr, nullptr);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		printf("Pressed key a: %c - %d | %d | %s | %d\n", key, key, scancode, (action == 1 ? "pressed" : (action == 2 ? "holding" : "released")), mods);
	});

	GLuint texObject;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texObject);

	glBindTexture(GL_TEXTURE_2D, texObject); //bind the first texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //set its parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height;
	if (window != nullptr)
	{
		glfwMakeContextCurrent(window);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImFont* robotoFont = io.Fonts->AddFontFromFileTTF("fonts\\Roboto-Medium.ttf", 16);
		//io.Fonts->AddFontDefault();

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &width, &height);

			glViewport(0, 0, width, height);
			glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glEnable(GL_TEXTURE_2D);
			glLoadIdentity();

			if (cartridge->GetHeader().nintendoLogo[0] != 0)
			{
				glBindTexture(GL_TEXTURE_2D, texObject); //bind the first texture.
				float index[] = { 0.0, 1.0 };

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 2, index);
				glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 2, index);
				glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 2, index);
				glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 2, index);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_COLOR_INDEX, GL_BITMAP, &cartridge->GetHeader().nintendoLogo);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, 0.0f);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(128, 0.0f, 0.0f);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(128, 128, 0.0f);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 128, 0.0f);
				glEnd();
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_FLOAT, &cartridge->GetHeader().nintendoLogo);
			}

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();

			ImGui::NewFrame();

			//ImGui::PushFont(robotoFont);
			renderMenu();
			renderCartridge(cartridge);
			//ImGui::PopFont();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	
	ImGui_ImplOpenGL3_Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	system("pause");
	return 0;
}*/

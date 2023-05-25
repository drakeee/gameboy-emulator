#pragma once

#include <Main.h>

#include <thread>
#include <mutex>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_sdlrenderer.h>

namespace GameBoy
{
	class Emulator;
}

namespace GUI
{
	class Layer;
	class Application
	{
	public:
		static Application& Get()
		{
			static Application instance;
			return instance;
		}

	public:
		
		void SetTitle(const char* title);
		void SetSize(int width, int height);
		inline void SetMaxFPS(uint16_t fps) { this->m_FPS = fps; }

		bool LoadROM(std::string fileName);


		void OnUpdate();


		inline SDL_Window* GetWindow() { return this->m_Window; }
		inline SDL_Renderer* GetRenderer() { return this->m_Renderer; }
		inline std::vector<GameBoy::Emulator*>& GetAllEmulators() { return this->m_EmulatorStack; }

		inline void PushLayer(Layer* layer) { this->m_LayerStack.push_back(layer); }

	public:
		bool shouldClose = false;
		SDL_Color backgroundColor{ 150, 150, 150, 255 };

		GameBoy::Emulator* currentEmulator = nullptr;

	private:
		void SetStyle();

		SDL_Window* m_Window = nullptr;
		SDL_Renderer* m_Renderer = nullptr;

		std::string m_TitleName{ "My Precious GameBoy Emulator" };
		uint32_t m_FPS = 60;

		std::vector<Layer*> m_LayerStack;
		std::vector<GameBoy::Emulator*> m_EmulatorStack;

		std::thread m_EmulatorThread;
		std::mutex m_ThreadMutex;

	private:
		Application();
		~Application()
		{
			m_EmulatorThread.join();
		}

		void ProcessEmulator();
		
	public:
		Application(Application const&) = delete;
		void operator=(Application const&) = delete;
	};
}
#pragma once

#include <Main.h>

struct SDL_Keysym;

namespace GUI
{
	class Application;
	class Layer
	{
	public:
		Layer(Application* app, char* titleName = "Layer") : m_App(app), m_TitleName(titleName) {};
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnImGuiRender() {}
		virtual void OnKeyDown(const uint8_t* keys) {}

	protected:
		Application* m_App;
		std::string m_TitleName{ "" };
	};
}
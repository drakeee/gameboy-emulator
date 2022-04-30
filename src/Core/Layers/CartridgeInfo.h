#pragma once

#include <Core/Layer.h>

namespace GUI
{
	class CartridgeInfoLayer : public Layer
	{
		using Layer::Layer;

	public:
		void OnAttach() override;
		void OnImGuiRender() override;
		void OnKeyDown(const uint8_t* keys) override;

		void RenderNintendoLogo();

	private:
		int m_NintendoWidth = 48;
		int m_NintendoHeight = 8;

		SDL_Texture* m_Texture;
	};
}
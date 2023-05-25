#pragma once

#include <Core/Layer.h>

namespace GUI
{
	class GameTilesLayer : public Layer
	{
		using Layer::Layer;

	public:
		void OnAttach() override;
		void OnImGuiRender() override;
		void OnKeyDown(const uint8_t* keys) override;

	private:
		SDL_Texture* m_Texture = nullptr;

		void RenderTestTile();
	};
}
#pragma once

#include <Core/Layer.h>

namespace GUI
{
	class MenuBarLayer : public Layer
	{
		using Layer::Layer;

		void OnImGuiRender() override;
		void OnKeyDown(const uint8_t* keys) override;

		void OpenFile();
		void CloseWindow();
	};
}
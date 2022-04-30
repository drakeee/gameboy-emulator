#pragma once

#include <Core/Layer.h>

namespace GUI
{
	class MemoryInfoLayer : public Layer
	{
		using Layer::Layer;

	public:
		void OnAttach() override;
		void OnImGuiRender() override;
		void OnKeyDown(const uint8_t* keys) override;
	};
}
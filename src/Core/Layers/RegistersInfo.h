#pragma once

#include <Core/Layer.h>

namespace GUI
{
	class RegistersInfoLayer : public Layer
	{
		using Layer::Layer;

	public:
		void OnAttach() override;
		void OnImGuiRender() override;
		void OnKeyDown(const uint8_t* keys) override;
	};
}
#pragma once
#include <vector>
#include "Window.hpp"
#include "Layer.hpp"



namespace airs
{
	class LayeredWindow : public Window
	{
		friend class Layer;

	private:
		std::vector<Layer*> Layers;
		void PerformClosing();

	protected:
		virtual void OnMouseDown(int32_t x, int32_t y, key k) override;
		virtual void OnMouseMove(int32_t x, int32_t y) override;
		virtual void OnMouseUp(int32_t x, int32_t y, key k) override;
		virtual void OnMouseWheel(int32_t x, int32_t y, float d) override;
		virtual void OnMouseHWheel(int32_t x, int32_t y, float d) override;
		virtual void OnKeyDown(key k) override;
		virtual void OnKeyUp(key k) override;
		virtual void OnChar(char32_t c) override;
		virtual void OnUniChar(char32_t c) override;

	public:
		using Window::Window;

		bool AddLayer(Layer& layer);
	};
}
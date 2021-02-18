#pragma once
#include "../Window.hpp"
#include "Element.hpp"
#include "Renderer.hpp"
#include <vector>



namespace airs::gi
{
	class ContainerWindow : public Window
	{
	private:
		std::vector<Element*> Elements;
		vec2i LastMousePt;

	protected:
		virtual void OnResize(std::int32_t w, std::int32_t h) override;
		virtual void OnMouseDown(std::int32_t x, std::int32_t y, Key k) override;
		virtual void OnMouseUp(std::int32_t x, std::int32_t y, Key k) override;
		virtual void OnMouseWheel(std::int32_t x, std::int32_t y, float d) override;
		virtual void OnMouseHWheel(std::int32_t x, std::int32_t y, float d) override;
		virtual void OnMouseMove(std::int32_t x, std::int32_t y) override;
		virtual void OnKeyDown(Key k) override;
		virtual void OnKeyUp(Key k) override;
		virtual void OnChar(char32_t c) override;
		virtual void OnUniChar(char32_t c) override;

	public:
		using Window::Window;

		void Add(Element *element);
		void Remove(Element *element);

		void Render(Renderer& renderer);
	};
}
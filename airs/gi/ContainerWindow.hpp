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

	protected:
		virtual void OnResize(std::int32_t w, std::int32_t h) override;
		virtual void OnMouseDown(std::int32_t x, std::int32_t y, key k) override;
		virtual void OnMouseUp(std::int32_t x, std::int32_t y, key k) override;
		virtual void OnMouseWheel(std::int32_t x, std::int32_t y, float d) override;
		virtual void OnMouseHWheel(std::int32_t x, std::int32_t y, float d) override;
		virtual void OnMouseMove(std::int32_t x, std::int32_t y) override;
		virtual void OnKeyDown(key k) override;
		virtual void OnKeyUp(key k) override;
		virtual void OnChar(char32_t c) override;
		virtual void OnUniChar(char32_t c) override;

	public:
		//ContainerWindow(const std::string& title, int32_t w, int32_t h, int32_t x = UseDefault, int32_t y = UseDefault, Style s = Style::Default, StyleEx sx = StyleEx::Default);
		using Window::Window;

		void Add(Element& element);
		void Remove(Element& element);

		void Render(Renderer& renderer);
	};
}
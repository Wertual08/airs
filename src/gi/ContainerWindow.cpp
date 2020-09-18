#include "airs/gi/ContainerWindow.hpp"



namespace airs::gi
{
	void ContainerWindow::OnResize(std::int32_t w, std::int32_t h)
	{
		std::size_t i = Elements.size();
		while (i--) Elements[i]->ApplyConstraints(0, { w, h });
		Window::OnResize(w, h);
	}
	void ContainerWindow::OnMouseDown(std::int32_t x, std::int32_t y, key k)
	{
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnMouseDown({ x, y }, k)) return;
		Window::OnMouseDown(x, y, k);
	}
	void ContainerWindow::OnMouseUp(std::int32_t x, std::int32_t y, key k)
	{
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnMouseUp({ x, y }, k)) return;
		Window::OnMouseUp(x, y, k);
	}
	void ContainerWindow::OnMouseWheel(std::int32_t x, std::int32_t y, float d)
	{
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnMouseWheel({ x, y }, d)) return;
		Window::OnMouseWheel(x, y, d);
	}
	void ContainerWindow::OnMouseHWheel(std::int32_t x, std::int32_t y, float d)
	{
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnMouseHWheel({ x, y }, d)) return;
		Window::OnMouseHWheel(x, y, d);
	}
	void ContainerWindow::OnMouseMove(std::int32_t x, std::int32_t y)
	{
		bool unhandled = true;
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnMouseMove({ x, y })) unhandled = false;
		if (unhandled) Window::OnMouseMove(x, y);
	}
	void ContainerWindow::OnKeyDown(key k)
	{
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnKeyDown(k)) return;
		Window::OnKeyDown(k);
	}
	void ContainerWindow::OnKeyUp(key k)
	{
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnKeyUp(k)) return;
		Window::OnKeyUp(k);
	}
	void ContainerWindow::OnChar(char32_t c)
	{
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnChar(c)) return;
		Window::OnChar(c);
	}
	void ContainerWindow::OnUniChar(char32_t c)
	{
		std::size_t i = Elements.size();
		while (i--) if (Elements[i]->OnChar(c)) return;
		Window::OnUniChar(c);
	}

	void ContainerWindow::Add(Element& element)
	{
		Elements.push_back(&element);
	}
	void ContainerWindow::Remove(Element& element)
	{
		std::size_t j = 0;
		for (std::size_t i = 0; i < Elements.size(); i++)
			if (Elements[i] != &element) Elements[j++] = Elements[i];
		Elements.resize(j);
	}

	void ContainerWindow::Render(Renderer& renderer)
	{
		for (Element* element : Elements) element->Render(renderer);
	}

}
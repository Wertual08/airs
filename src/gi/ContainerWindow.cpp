#include "airs/gi/ContainerWindow.hpp"



namespace airs::gi
{
	void ContainerWindow::OnResize(std::int32_t w, std::int32_t h)
	{
		std::size_t i = Elements.size();
		while (i--) Elements[i]->ApplyConstraints(airs::vec2i(0), { w, h });
		Window::OnResize(w, h);
	}
	void ContainerWindow::OnMouseDown(std::int32_t x, std::int32_t y, Key k)
	{
		vec2i pos(x, y);
		vec2i del = pos - LastMousePt;
		LastMousePt = pos;

		MouseEventArgs args(pos, del, { 0, 0 }, k);
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendMouseDown(args);
		if (!args.Handled()) Window::OnMouseDown(x, y, k);
	}
	void ContainerWindow::OnMouseUp(std::int32_t x, std::int32_t y, Key k)
	{
		vec2i pos(x, y);
		vec2i del = pos - LastMousePt;
		LastMousePt = pos;

		MouseEventArgs args(pos, del, { 0, 0 }, k);
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendMouseUp(args);
		if (!args.Handled()) Window::OnMouseUp(x, y, k);
	}
	void ContainerWindow::OnMouseWheel(std::int32_t x, std::int32_t y, float d)
	{
		vec2i pos(x, y);
		vec2i del = pos - LastMousePt;
		LastMousePt = pos;

		MouseEventArgs args(pos, del, { 0, d }, Key::None);
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendMouseWheel(args);
		if (!args.Handled()) Window::OnMouseWheel(x, y, d);
	}
	void ContainerWindow::OnMouseHWheel(std::int32_t x, std::int32_t y, float d)
	{
		vec2i pos(x, y);
		vec2i del = pos - LastMousePt;
		LastMousePt = pos;

		MouseEventArgs args(pos, del, { d, 0 }, Key::None);
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendMouseWheel(args);
		if (!args.Handled()) Window::OnMouseHWheel(x, y, d);
	}
	void ContainerWindow::OnMouseMove(std::int32_t x, std::int32_t y)
	{
		vec2i pos(x, y);
		vec2i del = pos - LastMousePt;
		LastMousePt = pos;

		MouseEventArgs args(pos, del, { 0, 0 }, Key::None);
		bool unhandled = true;
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendMouseMove(args);
		if (!args.Handled()) Window::OnMouseMove(x, y);
	}
	void ContainerWindow::OnKeyDown(Key k)
	{
		KeyboardEventArgs args(k, 0);
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendKeyDown(args);
		if (!args.Handled()) Window::OnKeyDown(k);
	}
	void ContainerWindow::OnKeyUp(Key k)
	{
		KeyboardEventArgs args(k, 0);
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendKeyUp(args);
		if (!args.Handled()) Window::OnKeyUp(k);
	}
	void ContainerWindow::OnChar(char32_t c)
	{
		KeyboardEventArgs args(Key::None, c);
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendChar(args);
		if (!args.Handled()) Window::OnChar(c);
	}
	void ContainerWindow::OnUniChar(char32_t c)
	{
		KeyboardEventArgs args(Key::None, c);
		std::size_t i = Elements.size();
		while (i--) Elements[i]->SendChar(args);
		Window::OnUniChar(c);
	}

	void ContainerWindow::Add(Element *element)
	{
		Elements.push_back(element);
		element->ApplyConstraints(airs::vec2i(0), Size);
	}
	void ContainerWindow::Remove(Element *element)
	{
		std::size_t j = 0;
		for (std::size_t i = 0; i < Elements.size(); i++)
			if (Elements[i] != element) Elements[j++] = Elements[i];
		Elements.resize(j);
	}

	void ContainerWindow::Render(Renderer& renderer)
	{
		for (Element* element : Elements) element->Render(renderer);
	}

}
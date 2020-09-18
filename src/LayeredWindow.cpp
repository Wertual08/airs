#include "airs/LayeredWindow.hpp"



namespace airs
{
	void LayeredWindow::PerformClosing()
	{
		size_t j = 0;
		for (size_t i = 0; i < Layers.size(); i++)
			if (Layers[i]->Opened()) Layers[j++] = Layers[i];
		Layers.resize(j);
	}

	void LayeredWindow::OnMouseDown(int32_t x, int32_t y, key k)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnMouseDown(x, y, k)) return;
		Window::OnMouseDown(x, y, k);
	}
	void LayeredWindow::OnMouseMove(int32_t x, int32_t y)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnMouseMove(x, y)) return; 
		Window::OnMouseMove(x, y);
	}
	void LayeredWindow::OnMouseUp(int32_t x, int32_t y, key k)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnMouseUp(x, y, k)) return;
		Window::OnMouseUp(x, y, k);
	}
	void LayeredWindow::OnMouseWheel(int32_t x, int32_t y, float d)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnMouseWheel(d)) return;
		Window::OnMouseWheel(x, y, d);
	}
	void LayeredWindow::OnMouseHWheel(int32_t x, int32_t y, float d)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnMouseHWheel(d)) return;
		Window::OnMouseHWheel(x, y, d);
	}
	void LayeredWindow::OnKeyDown(key k)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnKeyDown(k)) return; 
		Window::OnKeyDown(k);
	}
	void LayeredWindow::OnKeyUp(key k)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnKeyUp(k)) return; 
		Window::OnKeyUp(k);
	}
	void LayeredWindow::OnChar(char32_t c)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnChar(c)) return; 
		Window::OnChar(c);
	}
	void LayeredWindow::OnUniChar(char32_t c)
	{
		size_t i = Layers.size();
		while (i--) if (Layers[i]->OnUniChar(c)) return;
		Window::OnUniChar(c);
	}

	bool LayeredWindow::AddLayer(Layer& layer)
	{
		if (!layer.Activate(this)) return false;
		Layers.push_back(&layer);
		return true;
	}
}
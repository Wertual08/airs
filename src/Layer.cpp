#include "airs/Layer.hpp"
#include "airs/LayeredWindow.hpp"



namespace airs
{
	bool Layer::Activate(LayeredWindow* window)
	{
		if (ActiveWindow || !OnActivate()) return false;
		ActiveWindow = window;
		return true;
	}

	bool Layer::OnActivate()
	{
		return true;
	}
	bool Layer::OnClose()
	{
		return true;
	}
	bool Layer::OnFocus()
	{
		return false;
	}
	bool Layer::OnKillFocus()
	{
		return false;
	}
	bool Layer::OnMouseDown(int32_t x, int32_t y, key k)
	{
		return false;
	}
	bool Layer::OnMouseMove(int32_t x, int32_t y)
	{
		return false;
	}
	bool Layer::OnMouseUp(int32_t x, int32_t y, key k)
	{
		return false;
	}
	bool Layer::OnMouseWheel(float d)
	{
		return false;
	}
	bool Layer::OnMouseHWheel(float d)
	{
		return false;
	}
	bool Layer::OnKeyDown(key k)
	{
		return false;
	}
	bool Layer::OnKeyUp(key k)
	{
		return false;
	}
	bool Layer::OnChar(char32_t c)
	{
		return false;
	}
	bool Layer::OnUniChar(char32_t c)
	{
		return false;
	}
	
	Layer::Layer() : ActiveWindow(nullptr)
	{
	}
	
	bool Layer::Close()
	{
		if (!ActiveWindow || !OnClose()) return false;
		ActiveWindow->PerformClosing();
		return true;
	}
	bool Layer::Closed() const
	{
		return !ActiveWindow;
	}
	bool Layer::Opened() const
	{
		return ActiveWindow;
	}
}
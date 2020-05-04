#include "airs/Control.h"



namespace airs
{
	Control::Control() : Control(0, 0)
	{
	}
	Control::Control(const Control& c)
	{
		Position = c.Position;
		Size = c.Size;
		LastLayers = c.LastLayers;
		Dirty = c.Dirty;
	}
	Control::Control(vec2i pos, vec2i size) : Position(pos), Size(size), LastLayers(0), Dirty(true), Visible(true)
	{
	}

	bool Control::UnderPoint(float x, float y) const
	{
		vec2i off = vec2i(x, y) - Position;
		return off >= vec2i(0) && off < Size;
	}
	bool Control::UnderPoint(vec2i vec) const
	{
		vec2i off = vec - Position;
		return off >= vec2i(0) && off < Size;
	}

	void Control::SetPosition(vec2i vec)
	{
		if (vec == Position) return;
		Position = vec;
		Dirty = true;
		if (Move) Move(Position);
	}
	vec2i Control::GetPosition() const
	{
		return Position;
	}
	void Control::AdjustPosition(vec2i vec)
	{
		SetPosition(Position + vec);
	}
	void Control::SetSize(vec2i vec)
	{
		if (vec == Size) return;
		Size = vec;
		Dirty = true;
		if (Resize) Resize(Size);
	}
	vec2i Control::GetSize() const
	{
		return Size;
	}
	void Control::AdjustSize(vec2i vec)
	{
		SetSize(Size + vec);
	}
	void Control::SetVisible(bool v)
	{
		Visible = v;
		Dirty = true;
	}
	bool Control::GetVisible() const
	{
		return Visible;
	}
	void Control::Refresh()
	{
		Dirty = true;
	}

	uint32_t Control::Layers() const
	{
		return 0;
	}

	void Control::OnMouseDown(MouseEvent& e)
	{
	}
	void Control::OnMouseMove(MouseEvent& e)
	{
	}
	void Control::OnMouseUp(MouseEvent& e)
	{
	}

	void Control::OnChar(KeyboardEvent& e)
	{
	}
	void Control::OnKeyDown(KeyboardEvent& e)
	{
	}
	void Control::OnKeyUp(KeyboardEvent& e)
	{
	}
	
	void Control::Render(Graphics& gfx) const
	{
	}
	
	void Control::RequestRender(Graphics& gfx)
	{
		size_t l = Visible ? Layers() : 0;
		if (LastLayers != l)
		{
			gfx.RequestRefresh();
			LastLayers = l;
		}

		if (l && (Dirty || gfx.RefreshRequested()))
		{
			Render(gfx);
			Dirty = false;
		}
		else gfx.Skip(l);
	}
}
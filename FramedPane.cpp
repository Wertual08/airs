#include "airs/FramedPane.h"



namespace airs
{
	FramedPane::FramedPane(vec2i pos, vec2i size, float width) : 
		Control(pos, size), Units{ GfxUnit() }, Widths(width), PressedSection(None)
	{
	}
	FramedPane::FramedPane(vec2i pos, vec2i size, float uw, float lw, float dw, float rw) : 
		Control(pos, size), Units{ GfxUnit() }, Widths(uw, lw, dw, rw), PressedSection(None)
	{
	}
	FramedPane::FramedPane(vec2i pos, vec2i size, float width, GfxUnit u0) :
		Control(pos, size), Widths(width), PressedSection(None)
	{
		for (uint32_t i = 0; i < 9; i++)
		{
			Units[i] = u0;
			u0.ID++;
		}
	}
	FramedPane::FramedPane(vec2i pos, vec2i size, float uw, float lw, float dw, float rw, GfxUnit u0 ) :
		Control(pos, size), Widths(uw, lw, dw, rw), PressedSection(None)
	{
		for (uint32_t i = 0; i < 9; i++)
		{
			Units[i] = u0;
			u0.ID++;
		}
	}

	void FramedPane::SetUnit(Section sect, GfxUnit unit)
	{
		Units[sect] = unit;
		Refresh();
	}
	GfxUnit FramedPane::GetUnit(Section sect) const
	{
		return Units[sect];
	}

	uint32_t FramedPane::Layers() const
	{
		return 9;
	}

	void FramedPane::OnMouseDown(MouseEvent& e)
	{
		if (e.Handled()) return;
		if (e.Key() == key::LButton && UnderPoint(e.Local()))
		{
			LastMouse = e.Global();

			vec2i loc = e.Local() - Position;
			if (loc >= vec2i(Widths.y, Widths.z) && loc < Size - vec2i(Widths.w, Widths.x))  PressedSection = Middle;
			else if (loc >= vec2i(Widths.y, Size.y - Widths.x) && loc < Size - vec2i(Widths.w, 0)) PressedSection = TopMiddle;
			else if (loc >= vec2i(0, Size.y - Widths.x) && loc < vec2i(Widths.y, Size.y)) PressedSection = TopLeft;
			else if (loc >= vec2i(0, Widths.z) && loc < vec2i(Widths.y, Size.y - Widths.x)) PressedSection = LeftMiddle;
			else if (loc >= vec2i(0) && loc < vec2i(Widths.y, Widths.z)) PressedSection = LeftBottom;
			else if (loc >= vec2i(Widths.y, 0) && loc < vec2i(Size.x - Widths.w, Widths.z)) PressedSection = BottomMiddle;
			else if (loc >= vec2i(Size.x - Widths.w, 0) && loc < vec2i(Size.x, Widths.z)) PressedSection = BottomRight;
			else if (loc >= vec2i(Size.x - Widths.w, Widths.z) && loc < vec2i(Size.x, Size.y - Widths.x)) PressedSection = RightMiddle;
			else if (loc >= vec2i(Size.x - Widths.w, Size.y - Widths.x) && loc < Size) PressedSection = RightTop;

			e.Handle();
		}
	}
	void FramedPane::OnMouseMove(MouseEvent& e)
	{
		if (e.Handled()) return;
		if (PressedSection != None)
		{ 
			if (Drag) Drag(e.Global() - LastMouse, PressedSection);
			LastMouse = e.Global();
			e.Handle();
		}
	}
	void FramedPane::OnMouseUp(MouseEvent& e)
	{
		if (e.Handled()) return;
		if (PressedSection != None)
		{
			PressedSection = None;
			e.Handle();
		}
	}

	void FramedPane::Render(Graphics& layout) const
	{
		layout.Render(Position + vec2i(Widths.y, Widths.z), Size - vec2i(Widths.y, Widths.z) - vec2i(Widths.w, Widths.x), Units[0]);

		layout.Render(Position + vec2i(Widths.y, Size.y - Widths.x), vec2i(Size.x - Widths.y - Widths.w, Widths.x), Units[1]);

		layout.Render(Position + vec2i(0, Size.y - Widths.x), vec2i(Widths.y, Widths.x), Units[2]);

		layout.Render(Position + vec2i(0, Widths.z), vec2i(Widths.y, Size.y - Widths.x - Widths.z), Units[3]);

		layout.Render(Position, vec2i(Widths.y, Widths.z), Units[4]);

		layout.Render(Position + vec2i(Widths.y, 0), vec2i(Size.x - Widths.y - Widths.w, Widths.z), Units[5]);

		layout.Render(Position + vec2i(Size.x - Widths.w, 0), vec2i(Widths.w, Widths.z), Units[6]);

		layout.Render(Position + vec2i(Size.x - Widths.w, Widths.z), vec2i(Widths.w, Size.y - Widths.x - Widths.z), Units[7]);

		layout.Render(Position + vec2i(Size.x - Widths.w, Size.y - Widths.x), vec2i(Widths.w, Widths.x), Units[8]);
	}
}
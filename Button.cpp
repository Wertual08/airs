#include "airs/Button.h"
#include "airs/Utilities.h"



namespace airs
{
	Button::Button() : Button(0.0f, 0.0f)
	{
	}
	Button::Button(vec2i pos, vec2i size, uint32_t id) :
		Control(pos, size), MousePressing(false), Selected(false), PressedUnit(GfxUnit::Scaled, id, 0x808080ffu), 
		SelectedUnit(GfxUnit::Scaled, id, 0xd0d0d0ffu), ReleasedUnit(GfxUnit::Scaled, id, 0xffffffffu),
		PressedTextColor(0x808080ffu), SelectedTextColor(0xd0d0d0ff), ReleasedTextColor(0xffffffffu), TextOffset(0.0f)
	{
	}
	Button::Button(vec2i pos, vec2i size, GfxUnit punit, GfxUnit sunit, GfxUnit runit) : 
		Control(pos, size), MousePressing(false), Selected(false), PressedUnit(punit), SelectedUnit(sunit), ReleasedUnit(runit),
		PressedTextColor(0x808080ffu), SelectedTextColor(0xd0d0d0ff), ReleasedTextColor(0xffffffffu), TextOffset(0.0f)
	{
	}

	void Button::SetPressedUnit(GfxUnit unit)
	{
		PressedUnit = unit;
		Refresh();
	}
	GfxUnit Button::GetPressedUnit() const
	{
		return PressedUnit;
	}
	void Button::SetSelectedUnit(GfxUnit unit)
	{
		SelectedUnit = unit;
		Refresh();
	}
	GfxUnit Button::GetSelectedUnit() const
	{
		return SelectedUnit;
	}
	void Button::SetReleasedUnit(GfxUnit unit)
	{
		ReleasedUnit = unit;
		Refresh();
	}
	GfxUnit Button::GetReleasedUnit() const
	{
		return ReleasedUnit;
	}

	void Button::SetPressedTextColor(uint32_t c)
	{
		PressedTextColor = c;
		Refresh();
	}
	uint32_t Button::GetPressedTextColor() const
	{
		return PressedTextColor;
	}
	void Button::SetSelectedTextColor(uint32_t c)
	{
		SelectedTextColor = c;
		Refresh();
	}
	uint32_t Button::GetSelectedTextColor() const
	{
		return SelectedTextColor;
	}
	void Button::SetReleasedTextColor(uint32_t c)
	{
		ReleasedTextColor = c;
		Refresh();
	}
	uint32_t Button::GetReleasedTextColor() const
	{
		return ReleasedTextColor;
	}
	void Button::SetText(const std::string& text)
	{
		Text = std::move(to_utf32(text));
		Refresh(); 
	}
	std::string Button::GetText() const
	{
		return std::move(to_utf8(Text));
	}
	void Button::SetTextOffset(vec2i offset)
	{
		TextOffset = offset;
		Refresh();
	}
	vec2i Button::GetTextOffset() const
	{
		return TextOffset;
	}

	uint32_t Button::Layers() const
	{
		return 1 + Text.length();
	}

	void Button::OnMouseDown(MouseEvent& e)
	{
		if (e.Handled()) return;
		if (e.Key() != key::LButton) return;

		bool press = MousePressing;
		if (MousePressing = UnderPoint(e.Local()))
		{
			LastMouse = e.Global();
			if (Press) Press();
			e.Handle();
		}
		if (press != MousePressing) Refresh();
	}
	void Button::OnMouseMove(MouseEvent& e)
	{
		bool under = UnderPoint(e.Local());
		if (under != Selected)
		{
			Selected = under;
			Refresh();
		}

		if (e.Handled()) return;
		if (MousePressing)
		{
			if (Drag) Drag(e.Global() - LastMouse);
			LastMouse = e.Global();
			e.Handle();
		}
	}
	void Button::OnMouseUp(MouseEvent& e)
	{
		if (e.Handled()) return;
		if (e.Key() != key::LButton) return;

		if (MousePressing) if (Release) Release();

		if (MousePressing && UnderPoint(e.Local())) if (Click) Click();
		if (MousePressing)
		{
			MousePressing = false;
			Refresh();
			e.Handle();
		}
	}
	
	void Button::Render(Graphics& gfx) const
	{
		if (MousePressing)
		{
			gfx.Render(Position, Size, PressedUnit);
			if (Text.length()) gfx.RenderString(Position + TextOffset + vec2i(Size) / 2 - 
				gfx.MeasureString(Text) / 2, Text, PressedTextColor);
		}
		else if (Selected)
		{
			gfx.Render(Position, Size, SelectedUnit);
			if (Text.length()) gfx.RenderString(Position + TextOffset + vec2i(Size) / 2 -
				gfx.MeasureString(Text) / 2, Text, SelectedTextColor);
		}
		else
		{
			gfx.Render(Position, Size, ReleasedUnit);
			if (Text.length()) gfx.RenderString(Position + TextOffset + vec2i(Size) / 2 - 
				gfx.MeasureString(Text) / 2, Text, ReleasedTextColor);
		}
	}
}
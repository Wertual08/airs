#include "airs/TextBox.h"
#include "airs/Utilities.h"



namespace airs
{
	TextBox::TextBox(vec2i pos, vec2i size, const std::string& s, uint32_t color) :
		Control(pos, size), Text(std::move(to_utf32(s))), Color(color), Cursor(Text.length())
	{
	}

	void TextBox::SetText(const std::string& s)
	{
		Text = std::move(to_utf32(s));
		Refresh();
	}
	std::string TextBox::GetText() const
	{
		return std::move(to_utf8(Text));
	}

	uint32_t TextBox::Layers() const
	{
		return Text.length() + 1;
	}

	void TextBox::OnMouseDown(MouseEvent& e)
	{
		bool old = Focused;
		if (Focused = (UnderPoint(e.Local()) && !e.Handled()))
		{
			MousePt = e.Local();
			LocateRequest = true;
			Refresh();
			e.Handle();
		}
		if (old != Focused) Refresh();
	}
	void TextBox::OnMouseMove(MouseEvent& e)
	{
	}
	void TextBox::OnMouseUp(MouseEvent& e)
	{
	}

	void TextBox::OnChar(KeyboardEvent& e)
	{
		if (e.Handled()) return;
		if (!Focused) return;

		if (e.Char() == '\b')
		{
			if (Text.length() && Cursor > 0) Text.erase(Cursor-- - 1, 1);
		}
		else if (e.Char() == '\r') return;
		else
		{
			Text.insert(Cursor++, 1, e.Char());
		}

		e.Handle();
		Refresh();
	}
	void TextBox::OnKeyDown(KeyboardEvent& e)
	{
		if (e.Handled()) return;
		if (!Focused) return;

		switch (e.Key())
		{
		case key::Left: if (Cursor > 0) Cursor--; e.Handle(); Refresh(); break;
		case key::Right: if (Cursor < Text.length()) Cursor++; e.Handle(); Refresh(); break;
		default: break;
		}
	}

	void TextBox::Render(Graphics& gfx) const
	{
		int32_t oy = Size.y - gfx.LineHeight();
		if (LocateRequest)
		{
			Cursor = gfx.LocateCursor(MousePt, { Position.x, Position.y + oy }, Text);
			LocateRequest = false;
		}

		vec2i c = gfx.RenderString({ Position.x, Position.y + oy }, Cursor, Text, Color);
		if (Focused) gfx.RenderPane({ c.x, c.y }, { 2, gfx.LineHeight() }, Color);
		else gfx.RenderEmpty();
	}
}
#include "airs/Label.h"
#include "airs/Utilities.h"



namespace airs
{
	Label::Label() : Label(0, "")
	{
	}
	Label::Label(vec2i pos, const std::string& text, uint32_t color) : Control(pos, 0.0f), Text(std::move(to_utf32(text))), Color(color)
	{
	}

	void Label::SetColor(uint32_t c)
	{
		Color = c;
		Refresh();
	}
	uint32_t Label::GetColor() const
	{
		return Color;
	}
	void Label::SetText(const std::string& text)
	{
		Text = std::move(to_utf32(text));
		Refresh();
	}
	std::string Label::GetText() const
	{
		return std::move(to_utf8(Text));
	}

	uint32_t Label::Layers() const
	{
		return Text.length();
	}

	void Label::Render(Graphics& gfx) const
	{
		if (Text.length()) gfx.RenderString(Position, Text, Color);
	}
}

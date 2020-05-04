#pragma once
#include "Control.h"



namespace airs
{
	class Label : public Control
	{
	protected:
		std::u32string Text;
		uint32_t Color;

	public:
		Label();
		Label(vec2i pos, const std::string& text, uint32_t color = 0xffffffffu);

		void SetColor(uint32_t c);
		uint32_t GetColor() const;
		void SetText(const std::string& text);
		std::string GetText() const;

		virtual uint32_t Layers() const override;

		virtual void Render(Graphics& gfx) const override;
	};
}

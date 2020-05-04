#pragma once
#include "Control.h"



namespace airs
{
	class TextBox : public Control
	{
	private:
		std::u32string Text;
		uint32_t Color;
		vec2i MousePt;
		mutable bool LocateRequest;
		mutable uint32_t Cursor;
		
		bool Focused;

	public:
		TextBox(vec2i pos, vec2i size, const std::string& s = "", uint32_t color = 0xffffffffu);

		void SetText(const std::string& s);
		std::string GetText() const;

		virtual uint32_t Layers() const override;

		virtual void OnMouseDown(MouseEvent& e) override;
		virtual void OnMouseMove(MouseEvent& e) override;
		virtual void OnMouseUp(MouseEvent& e) override;

		virtual void OnChar(KeyboardEvent& e) override;
		virtual void OnKeyDown(KeyboardEvent& e) override;

		virtual void Render(Graphics& gfx) const override;

		delegate<void(std::string)> TextChanged;
	};
}


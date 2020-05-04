#pragma once
#include "Control.h"



namespace airs
{
	class Button : public Control
	{
	protected:
		bool MousePressing, Selected;

		GfxUnit PressedUnit, SelectedUnit, ReleasedUnit;

		uint32_t PressedTextColor, SelectedTextColor, ReleasedTextColor;
		std::u32string Text;
		vec2i TextOffset, LastMouse;

	public:
		Button();
		Button(vec2i pos, vec2i size, uint32_t id);
		Button(vec2i pos, vec2i size, GfxUnit punit = GfxUnit(0x808080ffu), GfxUnit sunit = GfxUnit(0xd0d0d0ff), GfxUnit runit = GfxUnit());

		void SetPressedUnit(GfxUnit unit);
		GfxUnit GetPressedUnit() const;
		void SetSelectedUnit(GfxUnit unit);
		GfxUnit GetSelectedUnit() const;
		void SetReleasedUnit(GfxUnit unit);
		GfxUnit GetReleasedUnit() const;

		void SetPressedTextColor(uint32_t c);
		uint32_t GetPressedTextColor() const;
		void SetSelectedTextColor(uint32_t c);
		uint32_t GetSelectedTextColor() const;
		void SetReleasedTextColor(uint32_t c);
		uint32_t GetReleasedTextColor() const;
		void SetText(const std::string& text);
		std::string GetText() const;
		void SetTextOffset(vec2i offset);
		vec2i GetTextOffset() const;

		virtual uint32_t Layers() const override;

		virtual void OnMouseDown(MouseEvent& e) override;
		virtual void OnMouseMove(MouseEvent& e) override;
		virtual void OnMouseUp(MouseEvent& e) override;

		virtual void Render(Graphics& gfx) const override;

		delegate<void()> Click;
		delegate<void()> Press;
		delegate<void()> Release;
		delegate<void(vec2i)> Drag;
	};
}

#pragma once
#include "Control.h"



namespace airs
{
	class FramedPane : public Control
	{
	public:
		enum Section : int32_t
		{
			None = -1,
			Middle,
			TopMiddle,
			TopLeft,
			LeftMiddle,
			LeftBottom,
			BottomMiddle,
			BottomRight,
			RightMiddle,
			RightTop,

			N = -1,
			M,
			TM,
			TL,
			LM,
			LB,
			BM,
			BR,
			RM,
			RT,
		};

	protected:
		GfxUnit Units[9];
		vec4f Widths;
		vec2i LastMouse;
		Section PressedSection;

	public:
		FramedPane(vec2i pos, vec2i size, float width);
		FramedPane(vec2i pos, vec2i size, float uw, float lw, float dw, float rw);
		FramedPane(vec2i pos, vec2i size, float width, GfxUnit u0);
		FramedPane(vec2i pos, vec2i size, float uw, float lw, float dw, float rw, GfxUnit u0);

		void SetUnit(Section sect, GfxUnit unit);
		GfxUnit GetUnit(Section sect) const;

		uint32_t Layers() const override;

		virtual void OnMouseDown(MouseEvent& e) override;
		virtual void OnMouseMove(MouseEvent& e) override;
		virtual void OnMouseUp(MouseEvent& e) override;

		virtual void Render(Graphics& layout) const override;

		delegate<void(vec2i, Section)> Drag;
	};
}


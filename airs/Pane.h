#pragma once
#include "Control.h"



namespace airs
{
	class Pane : public Control
	{
	protected:
		GfxUnit Unit;

	public:
		Pane();
		Pane(vec2i pos, vec2i size, GfxUnit unit = GfxUnit());

		uint32_t Layers() const override;

		void SetUnit(GfxUnit unit);
		GfxUnit GetUnit() const;

		virtual void Render(Graphics& gfx) const override;
	};
}
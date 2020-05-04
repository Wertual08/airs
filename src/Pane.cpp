#include "airs/Pane.h"



namespace airs
{
	Pane::Pane() : Pane(0.0f, 0.0f)
	{
	}
	Pane::Pane(vec2i pos, vec2i size, GfxUnit unit) : Control(pos, size), Unit(unit)
	{
	}

	uint32_t Pane::Layers() const
	{
		return 1;
	}

	void Pane::SetUnit(GfxUnit unit)
	{
		Unit = unit;
		Refresh();
	}
	GfxUnit Pane::GetUnit() const
	{
		return Unit;
	}
	
	void Pane::Render(Graphics& gfx) const
	{
		gfx.Render(Position, Size, Unit);
	}
}
#pragma once
#include "Surface.hpp"
#include "Font.hpp"



namespace airs
{
	class Graphics
	{
	private:
		Surface& ActiveSurface;

	public:
		Graphics(Surface& surface);

		void Fill(uint32_t color);
		void Draw(int32_t x, int32_t y, const Surface& s);
		void Draw(int32_t x, int32_t y, const Glyph& s, uint32_t color);
		void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
	};
}
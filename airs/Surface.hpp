#pragma once
#include "math.hpp"



namespace airs
{
	class Surface
	{
	private:
		const bool OwnsMemory;

	protected:
		int32_t BitmapW;
		int32_t BitmapH;
		uint32_t* Pixels;

	public:
		Surface(int32_t w, int32_t h, void* data);
		Surface(int32_t w, int32_t h);
		Surface(const Surface& b);
		Surface(Surface&& b);
		Surface& operator=(const Surface& b);
		Surface() = delete;
		~Surface();

		int32_t Width() const;
		int32_t Height() const;
		size_t Size() const;

		uint32_t Pixel(int32_t x, int32_t y) const;
		uint32_t& Pixel(int32_t x, int32_t y);
		vec4ub Color(int32_t x, int32_t y) const;
		vec4ub& Color(int32_t x, int32_t y);
		operator const uint32_t* () const;
		operator uint32_t* ();
		operator const vec4ub* () const;
		operator vec4ub* ();
	};
}
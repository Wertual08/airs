#include "airs\Surface.hpp"



namespace airs
{
	Surface::Surface(int32_t w, int32_t h, void* data) : OwnsMemory(false),
		BitmapW(w), BitmapH(h), Pixels(static_cast<uint32_t*>(data))
	{
	}
	Surface::Surface(int32_t w, int32_t h) : OwnsMemory(true), BitmapW(w), BitmapH(h), Pixels(nullptr)
	{
		if (w > 0 && h > 0) Pixels = new uint32_t[static_cast<size_t>(w) * static_cast<size_t>(h)];
	}
	Surface::Surface(const Surface& b) : Surface(b.BitmapW, b.BitmapH)
	{
		std::memcpy(Pixels, b.Pixels, sizeof(uint32_t) * (static_cast<size_t>(BitmapW) * static_cast<size_t>(BitmapH)));
	}
	Surface::Surface(Surface&& b) : OwnsMemory(b.OwnsMemory), BitmapW(0), BitmapH(0), Pixels(nullptr)
	{
		std::swap(BitmapW, b.BitmapW);
		std::swap(BitmapH, b.BitmapH);
		std::swap(Pixels, b.Pixels);
	}
	Surface& Surface::operator=(const Surface& b)
	{
		if (!OwnsMemory || BitmapW != b.BitmapW || BitmapH != b.BitmapH)
		{
			if (OwnsMemory && Pixels) delete[] Pixels;
			Pixels = new uint32_t[static_cast<size_t>(b.BitmapW) * static_cast<size_t>(b.BitmapH)];
		}

		BitmapW = b.BitmapW;
		BitmapH = b.BitmapH;
		std::memcpy(Pixels, b.Pixels, sizeof(uint32_t) * (static_cast<size_t>(BitmapW) * static_cast<size_t>(BitmapH)));
		return *this;
	}
	Surface::~Surface()
	{
		if (OwnsMemory && Pixels)
		{
			delete[] Pixels;
			Pixels = nullptr;
		}
	}

	int32_t Surface::Width() const
	{
		return BitmapW;
	}
	int32_t Surface::Height() const
	{
		return BitmapH;
	}
	size_t Surface::Size() const
	{
		return static_cast<size_t>(BitmapW) * static_cast<size_t>(BitmapH);
	}

	uint32_t Surface::Pixel(int32_t x, int32_t y) const
	{
		return *(Pixels + static_cast<size_t>(y) * static_cast<size_t>(BitmapW) + static_cast<size_t>(x));
	}
	uint32_t& Surface::Pixel(int32_t x, int32_t y)
	{
		return *(Pixels + static_cast<size_t>(y) * static_cast<size_t>(BitmapW) + static_cast<size_t>(x));
	}
	vec4ub Surface::Color(int32_t x, int32_t y) const
	{
		return *reinterpret_cast<vec4ub*>(Pixels + static_cast<size_t>(y) * static_cast<size_t>(BitmapW) + static_cast<size_t>(x));
	}
	vec4ub& Surface::Color(int32_t x, int32_t y)
	{
		return *reinterpret_cast<vec4ub*>(Pixels + static_cast<size_t>(y) * static_cast<size_t>(BitmapW) + static_cast<size_t>(x));
	}
	Surface::operator const uint32_t* () const
	{
		return Pixels;
	}
	Surface::operator uint32_t* ()
	{
		return Pixels;
	}
	Surface::operator const vec4ub* () const
	{
		return reinterpret_cast<vec4ub*>(Pixels);
	}
	Surface::operator vec4ub* ()
	{
		return reinterpret_cast<vec4ub*>(Pixels);
	}
}
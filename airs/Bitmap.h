#pragma once
#include <string>
#include <memory>
#include <cstdint>


namespace airs
{
	class Bitmap
	{
	private:
		std::unique_ptr<uint32_t[]> Pixels;

	public:

		Bitmap(uint32_t w, uint32_t h);
		Bitmap(const std::string& file);
		Bitmap(const Bitmap& b);
		Bitmap& operator=(const Bitmap& b);
		~Bitmap();

		uint32_t Width() const;
		uint32_t Height() const;
		size_t Size() const;

		uint32_t Pixel(uint32_t x, uint32_t y) const;
		uint32_t& Pixel(uint32_t x, uint32_t y);
		operator const uint32_t* () const;
		operator uint32_t* ();
	};
}

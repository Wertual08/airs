#pragma once
#include "Surface.hpp"
#include <string>



namespace airs
{
	class Bitmap : public Surface
	{
	public:
		Bitmap(uint32_t w, uint32_t h, void* data);
		Bitmap(uint32_t w, uint32_t h);
		Bitmap(const std::string& file);
		Bitmap(const Bitmap& b) noexcept;
		Bitmap(Bitmap&& b) noexcept;
		Bitmap& operator=(const Bitmap& b);
	};
}

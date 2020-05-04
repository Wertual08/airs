#include "airs/Bitmap.h"
#include <Windows.h>
#include <gdiplus.h>
#include <stdexcept>



namespace airs
{
	//inline uint32_t BGRAtoRGBA

	Bitmap::Bitmap(uint32_t w, uint32_t h)
	{
		Pixels = std::make_unique<uint32_t[]>(static_cast<size_t>(w) * static_cast<size_t>(h) + 2ull);
		Pixels[0] = w;
		Pixels[1] = h;
	}
	Bitmap::Bitmap(const std::string& file)
	{
		wchar_t WideName[512];
		mbstowcs_s(nullptr, WideName, file.c_str(), _TRUNCATE);
		Gdiplus::Bitmap bitmap(WideName);
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
			throw std::runtime_error(("airs::Bitmap error: Failed to load bitmap from [" + file + "].").c_str());

		Pixels = std::make_unique<uint32_t[]>(static_cast<size_t>(bitmap.GetWidth()) * static_cast<size_t>(bitmap.GetHeight()) + 2ull);
		Pixels[0] = bitmap.GetWidth();
		Pixels[1] = bitmap.GetHeight();

		Gdiplus::BitmapData data;
		bitmap.LockBits(&Gdiplus::Rect(0, 0, bitmap.GetWidth(), bitmap.GetHeight()),
			Gdiplus::ImageLockMode::ImageLockModeRead,
			PixelFormat32bppARGB, &data); 
		uint32_t* udata = static_cast<uint32_t*>(data.Scan0);
		for (size_t y = 0; y < bitmap.GetHeight(); y++)
		{
			for (size_t x = 0; x < bitmap.GetWidth(); x++)
			{
				uint32_t p = udata[(bitmap.GetHeight() - 1 - y) * bitmap.GetWidth() + x];
				Pixels[y * bitmap.GetWidth() + x + 2ull] = (p << 8) | (p >> 24);
			}
		}
		//memcpy(Pixels.get(), data.Scan0, sizeof(uint32_t) * ImgWidth * ImgHeight);
		bitmap.UnlockBits(&data);
	}
	Bitmap::Bitmap(const Bitmap& b)
	{
		Pixels = std::make_unique<uint32_t[]>(static_cast<size_t>(b.Pixels[0]) * static_cast<size_t>(b.Pixels[1]) + 2ull);
		memcpy(Pixels.get(), b.Pixels.get(), sizeof(uint32_t) * (static_cast<size_t>(b.Pixels[0]) * static_cast<size_t>(b.Pixels[1]) + 2ull));
	}
	Bitmap& Bitmap::operator=(const Bitmap& b)
	{
		Pixels = std::make_unique<uint32_t[]>(static_cast<size_t>(b.Pixels[0]) * static_cast<size_t>(b.Pixels[1]) + 2ull);
		memcpy(Pixels.get(), b.Pixels.get(), sizeof(uint32_t) * (static_cast<size_t>(b.Pixels[0]) * static_cast<size_t>(b.Pixels[1]) + 2ull));
		return *this;
	}
	Bitmap::~Bitmap()
	{
	}

	uint32_t Bitmap::Width() const
	{
		return Pixels[0];
	}
	uint32_t Bitmap::Height() const
	{
		return Pixels[1];
	}
	size_t Bitmap::Size() const
	{
		return static_cast<size_t>(Pixels[0]) * static_cast<size_t>(Pixels[1]);
	}
	
	uint32_t Bitmap::Pixel(uint32_t x, uint32_t y) const
	{
		return *(Pixels.get() + 2ull + static_cast<size_t>(y) * static_cast<size_t>(Pixels[0]) + static_cast<size_t>(x));
	}
	uint32_t& Bitmap::Pixel(uint32_t x, uint32_t y)
	{
		return *(Pixels.get() + 2ull + static_cast<size_t>(y) * static_cast<size_t>(Pixels[0]) + static_cast<size_t>(x));
	}
	Bitmap::operator const uint32_t* () const
	{
		return Pixels.get() + 2ull;
	}
	Bitmap::operator uint32_t* ()
	{
		return Pixels.get() + 2ull;
	}
}
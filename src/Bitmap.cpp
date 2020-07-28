#include "airs/Bitmap.hpp"
#include <Windows.h>
#include <gdiplus.h>
#include <stdexcept>
#include "airs/Utilities.hpp"



namespace airs
{
	class GDIPlusInitializer
	{
		ULONG_PTR GDIPlusToken = 0;
	public:
		GDIPlusInitializer()
		{
			Gdiplus::GdiplusStartupInput StartupInput;
			Gdiplus::Status result = Gdiplus::GdiplusStartup(&GDIPlusToken, &StartupInput, nullptr);
			if (result != Gdiplus::Status::Ok) throw std::runtime_error("airs::GDIPlusInitializer error: Can not acquire GDI+ token.");
		}
		~GDIPlusInitializer()
		{
			Gdiplus::GdiplusShutdown(GDIPlusToken);
		}
		ULONG_PTR GetToken()
		{
			return GDIPlusToken;
		}
	};
	//inline uint32_t ARGBtoRGBA

	Bitmap::Bitmap(uint32_t w, uint32_t h, void* data) : Surface(w, h, data)
	{
	}
	Bitmap::Bitmap(uint32_t w, uint32_t h) : Surface(w, h)
	{
	}
	Bitmap::Bitmap(const std::string& file) : Surface(0, 0)
	{
		static GDIPlusInitializer GDIPlusInstance;

		Gdiplus::Bitmap bitmap(to_wide(file).c_str());
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
			throw std::runtime_error("airs::Bitmap error: Failed to load bitmap from [" + file + "].");

		BitmapW = bitmap.GetWidth();
		BitmapH = bitmap.GetHeight();
		Pixels = new uint32_t[static_cast<size_t>(BitmapW) * static_cast<size_t>(BitmapH)];

		Gdiplus::BitmapData data;
		Gdiplus::Rect rect(0, 0, BitmapW, BitmapH);
		bitmap.LockBits(&rect, Gdiplus::ImageLockMode::ImageLockModeRead,
			PixelFormat32bppARGB, &data); 
		uint32_t* udata = static_cast<uint32_t*>(data.Scan0);
		for (size_t y = 0; y < static_cast<size_t>(BitmapH); y++)
		{
			for (size_t x = 0; x < static_cast<size_t>(BitmapW); x++)
			{
				uint32_t p = udata[(BitmapH - 1ull - y) * BitmapW + x];
				Pixels[y * BitmapW + x] = static_cast<uint32_t>((p << 8) | (p >> 24));
			}
		}
		bitmap.UnlockBits(&data);
	}
	Bitmap::Bitmap(const Bitmap& b) noexcept : Surface(b)
	{
	}
	Bitmap::Bitmap(Bitmap&& b) noexcept : Surface(std::move(b))
	{
	}
	Bitmap& Bitmap::operator=(const Bitmap& b)
	{
		return static_cast<Bitmap&>(Surface::operator=(b));
	}
}
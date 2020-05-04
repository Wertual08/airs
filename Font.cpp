#include "airs/Font.h"
#include <stdexcept>
#include "airs/Utilities.h"
#include "AIRSWin.h"



namespace airs
{
	FIXED fl2fx(float v)
	{
		return { static_cast<uint16_t>((0xffff + 1.0f) * v), static_cast<int16_t>(v) };
	}

	Font::Font(const std::string& file, const std::string& font, int32_t size) :
		Font(file, font, size, 0, 0, FW_DONTCARE, false, false, false, FIXED_PITCH | FF_DONTCARE)
	{ 
	}
	Font::Font(const std::string& font, int32_t size) : Font("", font, size, 0, 0, FW_DONTCARE, false, false, false, FIXED_PITCH | FF_DONTCARE)
	{
	}
	Font::Font(const std::string& font) : Font("", font, 0, 0, 0, FW_DONTCARE, false, false, false, FIXED_PITCH | FF_DONTCARE)
	{
	}
	Font::Font(int32_t size) : Font("", "", size, 0, 0, FW_DONTCARE, false, false, false, FIXED_PITCH | FF_DONTCARE)
	{
	}
	Font::Font() : Font("", "", 0, 0, 0, FW_DONTCARE, false, false, false, FIXED_PITCH | FF_DONTCARE)
	{
	}
	Font::Font(const std::string& file, const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily)
	{
		if (file.length()) AddFontResourceExW((LPCWSTR)to_utf16(file).c_str(), FR_PRIVATE, 0);

		HDC FontDC = GetDC(0); 
		int k;
		SelectObject(FontDC, Handle = CreateFontW(size, 0, static_cast<int32_t>(escapement * 1800.0f / 3.141592f),
			static_cast<int32_t>(orientation * 1.8f / 3.141592f), weight, italic, underline, strikeout, ANSI_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_STROKE_PRECIS, PROOF_QUALITY, pitchandfamily, (LPCWSTR)to_utf16(font).c_str()));
		if (!Handle) throw std::runtime_error("airs::Font error: Font creation failed.");
		GetTextMetricsW(FontDC, reinterpret_cast<LPTEXTMETRICW>(&FontMetrics));
		ReleaseDC(0, FontDC);
		DeleteObject(FontDC);
	}
	Font::Font(const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily) :
		Font("", font, size, escapement, orientation, weight, italic, underline, strikeout, pitchandfamily)
	{
	}
	Font::Font(const std::string& file, const std::string& font, int32_t size, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily) :
		Font(file, font, size, 0, 0, weight, italic, underline, strikeout, pitchandfamily)
	{
	}
	Font::Font(const std::string& font, int32_t size, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily) :
		Font("", font, size, 0, 0, weight, italic, underline, strikeout, pitchandfamily)
	{
	}
	Font::Font(Font&& font) noexcept
	{
		std::swap(Handle, font.Handle);
		std::swap(FontMetrics, font.FontMetrics);
	}
	Font& Font::operator=(Font&& font) noexcept
	{
		std::swap(Handle, font.Handle);
		std::swap(FontMetrics, font.FontMetrics);
		return *this;
	}
	Font::~Font()
	{
		if (Handle) DeleteObject(Handle);
		Handle = 0;
	}

	int32_t Font::Height() const
	{
		return FontMetrics.Height;
	}
	int32_t Font::Ascent() const
	{
		return FontMetrics.Ascent;
	}
	int32_t Font::Descent() const
	{
		return FontMetrics.Descent;
	}
	int32_t Font::InternalLeading() const
	{
		return FontMetrics.InternalLeading;
	}
	int32_t Font::ExternalLeading() const
	{
		return FontMetrics.ExternalLeading;
	}
	int32_t Font::AveCharWidth() const
	{
		return FontMetrics.AveCharWidth;
	}
	int32_t Font::MaxCharWidth() const
	{
		return FontMetrics.MaxCharWidth;
	}
	int32_t Font::Weight() const
	{
		return FontMetrics.Weight;
	}
	int32_t Font::Overhang() const
	{
		return FontMetrics.Overhang;
	}
	int32_t Font::DigitizedAspectX() const
	{
		return FontMetrics.DigitizedAspectX;
	}
	int32_t Font::DigitizedAspectY() const
	{
		return FontMetrics.DigitizedAspectY;
	}
	char16_t Font::FirstChar() const
	{
		return FontMetrics.FirstChar;
	}
	char16_t Font::LastChar() const
	{
		return FontMetrics.LastChar;
	}
	char16_t Font::DefaultChar() const
	{
		return FontMetrics.DefaultChar;
	}
	char16_t Font::BreakChar() const
	{
		return FontMetrics.BreakChar;
	}
	uint8_t Font::Italic() const
	{
		return FontMetrics.Italic;
	}
	uint8_t Font::Underlined() const
	{
		return FontMetrics.Underlined;
	}
	uint8_t Font::StruckOut() const
	{
		return FontMetrics.StruckOut;
	}
	uint8_t Font::PitchAndFamily() const
	{
		return FontMetrics.PitchAndFamily;
	}
	uint8_t Font::CharSet() const
	{
		return FontMetrics.CharSet;
	}

	Font::operator void* () const
	{
		return Handle;
	}


	Glyph::Glyph(const Font& font, char32_t char_id, bool gray, mat2 t) noexcept
	{
		HDC FontDC = CreateCompatibleDC(0);
		SelectObject(FontDC, font);

		MAT2 Matrix;
		Matrix.eM11 = fl2fx(t[0][0]);
		Matrix.eM21 = fl2fx(t[1][0]);
		Matrix.eM22 = fl2fx(t[1][1]);
		Matrix.eM12 = fl2fx(t[0][1]);

		uint32_t format = gray ? GGO_GRAY8_BITMAP : GGO_BITMAP;
		
		GLYPHMETRICS gm;
		uint32_t size = GetGlyphOutlineW(FontDC, char_id, format, &gm, 0, 0, &Matrix);
		if (size != GDI_ERROR)
		{
			uint8_t* buffer = new uint8_t[size];
			GetGlyphOutlineW(FontDC, char_id, format, &gm, size, buffer, &Matrix);
			gm.gmBlackBoxX *= size > 0;
			gm.gmBlackBoxY *= size > 0;
			
			Dimensions.BitmapW = gm.gmBlackBoxX;
			Dimensions.BitmapH = gm.gmBlackBoxY;
			Dimensions.OriginX = gm.gmptGlyphOrigin.x;
			Dimensions.OriginY = font.Descent() + gm.gmptGlyphOrigin.y - Dimensions.BitmapH;
			Dimensions.PosIncX = gm.gmCellIncX;
			Dimensions.PosIncY = gm.gmCellIncY;
			PixData = std::make_unique<uint8_t[]>(sizeof(uint8_t) * gm.gmBlackBoxX * gm.gmBlackBoxY + sizeof(int32_t) * 2);
			((int32_t*)(PixData.get()))[0] = gm.gmBlackBoxX;
			((int32_t*)(PixData.get()))[1] = gm.gmBlackBoxY;

			if (gray)
			{
				uint32_t row = gm.gmBlackBoxX + 3 - (gm.gmBlackBoxX + 3) % 4;
				for (uint32_t y = 0; y < gm.gmBlackBoxY; y++)
					for (uint32_t x = 0; x < gm.gmBlackBoxX; x++)
						PixData[sizeof(int32_t) * 2 + y * gm.gmBlackBoxX + x] = 
						buffer[(gm.gmBlackBoxY - y - 1) * row + x] * 255 / 64;
			}
			else
			{
				int32_t row = (gm.gmBlackBoxX + 31 - (gm.gmBlackBoxX + 31) % 32) / 8;
				for (int32_t y = 0; y < gm.gmBlackBoxY; y++)
					for (int32_t x = 0; x < gm.gmBlackBoxX; x++)
						PixData[sizeof(int32_t) * 2 + y * gm.gmBlackBoxX + x] = 
						((buffer[x / 8 + row * (gm.gmBlackBoxY - y - 1)] >> (7 - x % 8)) & 1) * 255;
			}

			delete[] buffer;

			DeleteDC(FontDC);
		}
		else
		{
			Dimensions.BitmapW = 0;
			Dimensions.BitmapH = 0;
			Dimensions.OriginX = 0;
			Dimensions.OriginY = 0;
			Dimensions.PosIncX = 0;
			Dimensions.PosIncY = 0;
			PixData = std::make_unique<uint8_t[]>(sizeof(int32_t) * 2);
			((int32_t*)(PixData.get()))[0] = 0;
			((int32_t*)(PixData.get()))[1] = 0;
		}
	}
	Glyph::Glyph(Glyph&& g) noexcept
	{
		std::swap(Dimensions.OriginX, g.Dimensions.OriginX);
		std::swap(Dimensions.OriginY, g.Dimensions.OriginY);
		std::swap(Dimensions.PosIncX, g.Dimensions.PosIncX);
		std::swap(Dimensions.PosIncY, g.Dimensions.PosIncY);
		PixData.swap(g.PixData);
	}
	Glyph& Glyph::operator=(Glyph&& g) noexcept
	{
		std::swap(Dimensions.OriginX, g.Dimensions.OriginX);
		std::swap(Dimensions.OriginY, g.Dimensions.OriginY);
		std::swap(Dimensions.PosIncX, g.Dimensions.PosIncX);
		std::swap(Dimensions.PosIncY, g.Dimensions.PosIncY);
		PixData.swap(g.PixData);
		return *this;
	}
	Glyph::Glyph(const Glyph& g)
	{
		Dimensions = g.Dimensions;
		PixData = std::make_unique<uint8_t[]>(sizeof(uint8_t) * ((int32_t*)(g.PixData.get()))[0] * ((int32_t*)(g.PixData.get()))[1] + 2 * sizeof(int32_t));
		memcpy(PixData.get(), g.PixData.get(), sizeof(uint8_t) * ((int32_t*)(g.PixData.get()))[0] * ((int32_t*)(g.PixData.get()))[1] + 2 * sizeof(int32_t));
	}
	Glyph& Glyph::operator=(const Glyph& g)
	{
		Dimensions = g.Dimensions;
		PixData = std::make_unique<uint8_t[]>(sizeof(uint8_t) * ((int32_t*)(g.PixData.get()))[0] * ((int32_t*)(g.PixData.get()))[1] + 2 * sizeof(int32_t));
		memcpy(PixData.get(), g.PixData.get(), sizeof(uint8_t) * ((int32_t*)(g.PixData.get()))[0] * ((int32_t*)(g.PixData.get()))[1] + 2 * sizeof(int32_t));
		return *this;
	}
	Glyph::~Glyph()
	{
	}

	int32_t Glyph::Size() const
	{
		return Dimensions.BitmapW * Dimensions.BitmapH;
	}
	int32_t Glyph::BitmapW() const
	{
		return Dimensions.BitmapW;
	}
	int32_t Glyph::BitmapH() const
	{
		return Dimensions.BitmapH;
	}
	int32_t Glyph::OriginX() const
	{
		return Dimensions.OriginX;
	}
	int32_t Glyph::OriginY() const
	{
		return Dimensions.OriginY;
	}
	int16_t Glyph::PosIncX() const
	{
		return Dimensions.PosIncX;
	}
	int16_t Glyph::PosIncY() const
	{
		return Dimensions.PosIncY;
	}
	Glyph::Metrics Glyph::GetMetrics() const
	{
		return Dimensions;
	}
	Glyph::operator const uint8_t* () const
	{
		return PixData.get() + sizeof(int32_t) * 2;
	}

	int32_t Glyph::Size(const Font& f, char32_t char_id, bool gray, mat2 t)
	{
		HDC FontDC = CreateCompatibleDC(0);
		SelectObject(FontDC, f);

		MAT2 Matrix;
		Matrix.eM11 = fl2fx(t[0][0]);
		Matrix.eM21 = fl2fx(t[1][0]);
		Matrix.eM22 = fl2fx(t[1][1]);
		Matrix.eM12 = fl2fx(t[0][1]);

		GLYPHMETRICS gm;
		DWORD size = GetGlyphOutlineW(FontDC, char_id, gray ? GGO_GRAY8_BITMAP : GGO_BITMAP, &gm, 0, 0, &Matrix);
		gm.gmBlackBoxX *= size > 0;
		gm.gmBlackBoxY *= size > 0;

		ReleaseDC(0, FontDC);
		DeleteObject(FontDC);
		return gm.gmBlackBoxX * gm.gmBlackBoxY;
	}
}

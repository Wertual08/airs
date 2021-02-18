#include "airs/Font.hpp"
#include <stdexcept>
#include "airs/encoding.hpp"
#include "AIRSWin.hpp"



namespace airs
{
	inline FIXED fl2fx(float v)
	{
		return { static_cast<uint16_t>((0xffff + 1.0f) * v), static_cast<int16_t>(v) };
	}
	
	
	Glyph::Glyph(std::unique_ptr<uint8_t[]>& data, const Metrics& dimensions) :
		PixData(std::move(data)), Dimensions(dimensions)
	{
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


	Font::Font(void* data, size_t lenght, const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily)
	{
		DWORD num_fonts = 0;
		HANDLE resource = AddFontMemResourceEx(data, static_cast<DWORD>(lenght), nullptr, &num_fonts);
		if (!resource) throw std::runtime_error("airs::Font error: Adding font memory resource failed.");
		HDC FontDC = GetDC(0);
		SelectObject(FontDC, Handle = CreateFontW(size, 0, static_cast<int32_t>(escapement * 1800.0f / 3.141592f),
			static_cast<int32_t>(orientation * 1.8f / 3.141592f), weight, italic, underline, strikeout, ANSI_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_STROKE_PRECIS, PROOF_QUALITY, pitchandfamily, to_wide(font).c_str()));
		if (!Handle) throw std::runtime_error("airs::Font error: Font creation failed.");
		GetTextMetricsW(FontDC, reinterpret_cast<LPTEXTMETRICW>(&FontMetrics));
		ReleaseDC(0, FontDC);
		DeleteObject(FontDC);
		RemoveFontMemResourceEx(resource);
	}
	Font::Font(const std::string& file, const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily)
	{
		if (file.length()) AddFontResourceExW(to_wide(file).c_str(), FR_PRIVATE, 0);

		HDC FontDC = GetDC(0); 
		SelectObject(FontDC, Handle = CreateFontW(size, 0, static_cast<int32_t>(escapement * 1800.0f / 3.141592f),
			static_cast<int32_t>(orientation * 1.8f / 3.141592f), weight, italic, underline, strikeout, ANSI_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_STROKE_PRECIS, PROOF_QUALITY, pitchandfamily, to_wide(font).c_str()));
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

	std::int32_t Font::Size(char32_t char_id, bool gray, mat2f t) const
	{
		HDC FontDC = CreateCompatibleDC(0);
		SelectObject(FontDC, static_cast<HGDIOBJ>(Handle));

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
	Glyph Font::GetGlyph(char32_t char_id, bool gray, mat2f t) const
	{
		HDC FontDC = CreateCompatibleDC(0);
		SelectObject(FontDC, static_cast<HGDIOBJ>(Handle));

		MAT2 Matrix;
		Matrix.eM11 = fl2fx(t[0][0]);
		Matrix.eM21 = fl2fx(t[1][0]);
		Matrix.eM22 = fl2fx(t[1][1]);
		Matrix.eM12 = fl2fx(t[0][1]);

		std::uint32_t format = gray ? GGO_GRAY8_BITMAP : GGO_BITMAP;

		GLYPHMETRICS gm;
		std::uint32_t size = GetGlyphOutlineW(FontDC, char_id, format, &gm, 0, 0, &Matrix);
		if (size != GDI_ERROR)
		{
			std::uint8_t* buffer = new uint8_t[size];
			GetGlyphOutlineW(FontDC, char_id, format, &gm, size, buffer, &Matrix);
			gm.gmBlackBoxX *= size > 0;
			gm.gmBlackBoxY *= size > 0;

			Glyph::Metrics dim;
			dim.BitmapW = gm.gmBlackBoxX;
			dim.BitmapH = gm.gmBlackBoxY;
			dim.OriginX = gm.gmptGlyphOrigin.x;
			dim.OriginY = Descent() + gm.gmptGlyphOrigin.y - dim.BitmapH;
			dim.PosIncX = gm.gmCellIncX;
			dim.PosIncY = gm.gmCellIncY;
			auto data = std::make_unique<std::uint8_t[]>(sizeof(std::uint8_t) * gm.gmBlackBoxX * gm.gmBlackBoxY + sizeof(std::int32_t) * 2);
			(reinterpret_cast<std::int32_t*>(data.get()))[0] = gm.gmBlackBoxX;
			(reinterpret_cast<std::int32_t*>(data.get()))[1] = gm.gmBlackBoxY;

			if (gray)
			{
				std::size_t row = gm.gmBlackBoxX + 3ull - (gm.gmBlackBoxX + 3ull) % 4ull;
				for (std::size_t y = 0; y < gm.gmBlackBoxY; y++)
					for (std::size_t x = 0; x < gm.gmBlackBoxX; x++)
						data[sizeof(std::int32_t) * 2 + y * gm.gmBlackBoxX + x] =
						buffer[(gm.gmBlackBoxY - y - 1) * row + x] * 255 / 64;
			}
			else
			{
				std::size_t row = (gm.gmBlackBoxX + 31 - (gm.gmBlackBoxX + 31) % 32) / 8;
				for (std::size_t y = 0; y < gm.gmBlackBoxY; y++)
					for (std::size_t x = 0; x < gm.gmBlackBoxX; x++)
						data[sizeof(std::int32_t) * 2 + y * gm.gmBlackBoxX + x] =
						((buffer[x / 8 + row * (gm.gmBlackBoxY - y - 1)] >> (7 - x % 8)) & 1) * 255;
			}

			delete[] buffer;
			DeleteDC(FontDC);

			return Glyph(data, dim);
		}
		else
		{
			Glyph::Metrics dim;
			dim.BitmapW = 0;
			dim.BitmapH = 0;
			dim.OriginX = 0;
			dim.OriginY = 0;
			dim.PosIncX = 0;
			dim.PosIncY = 0;
			auto data = std::make_unique<uint8_t[]>(sizeof(int32_t) * 2);
			((int32_t*)(data.get()))[0] = 0;
			((int32_t*)(data.get()))[1] = 0;
			return Glyph(data, dim);
		}
	}
}

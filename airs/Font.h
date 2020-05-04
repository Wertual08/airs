#pragma once
#include "math.h"
#include <memory>
#include <cstdint>
#include <string>



namespace airs
{
	class Font
	{
	private:
		void* Handle;
		struct Metrics
		{
			int32_t Height;
			int32_t Ascent;
			int32_t Descent;
			int32_t InternalLeading;
			int32_t ExternalLeading;
			int32_t AveCharWidth;
			int32_t MaxCharWidth;
			int32_t Weight;
			int32_t Overhang;
			int32_t DigitizedAspectX;
			int32_t DigitizedAspectY;
			char16_t FirstChar;
			char16_t LastChar;
			char16_t DefaultChar;
			char16_t BreakChar;
			uint8_t Italic;
			uint8_t Underlined;
			uint8_t StruckOut;
			uint8_t PitchAndFamily;
			uint8_t CharSet;
		} FontMetrics;

	public:
		Font(const std::string& file, const std::string& font, int32_t size);
		Font(const std::string& font, int32_t size);
		Font(const std::string& font);
		Font(int32_t size);
		Font();
		Font(const std::string& file, const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(const std::string& file, const std::string& font, int32_t size, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(const std::string& font, int32_t size, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(Font&& font) noexcept;
		Font& operator=(Font&& font) noexcept;
		Font(const Font&) = delete;
		~Font();
		Font& operator=(const Font&) = delete;

		int32_t Height() const;
		int32_t Ascent() const;
		int32_t Descent() const;
		int32_t InternalLeading() const;
		int32_t ExternalLeading() const;
		int32_t AveCharWidth() const;
		int32_t MaxCharWidth() const;
		int32_t Weight() const;
		int32_t Overhang() const;
		int32_t DigitizedAspectX() const;
		int32_t DigitizedAspectY() const;
		char16_t FirstChar() const;
		char16_t LastChar() const;
		char16_t DefaultChar() const;
		char16_t BreakChar() const;
		uint8_t Italic() const;
		uint8_t Underlined() const;
		uint8_t StruckOut() const;
		uint8_t PitchAndFamily() const;
		uint8_t CharSet() const;

		operator void* () const;

		enum Weight : int32_t
		{
			DontCare =		0,
			Thin =			100,
			ExtraLight =	200,
			UltraLight =	200,
			Light =			300,
			Normal =		400,
			Regular =		400,
			Medium =		500,
			SemiBold =		600,
			DemiBold =		600,
			Bold =			700,
			ExtraBold =		800,
			UltraBold =		800,
			Heavy =			900,
			Black =			900,
		};
		enum Pitch : uint32_t
		{
			Default =		0, 
			Fixed =			1,			
			Variable =		2, 
		};
		enum Family : uint32_t
		{
			FDontCare =		0 << 4, 
			Roman =			1 << 4, 
			Swiss =			2 << 4, 
			Modern =		3 << 4, 
			Script =		4 << 4, 
			Decorative =	5 << 4, 
		};
	};

	class Glyph
	{
	public:
		struct Metrics
		{
			int32_t BitmapW;
			int32_t BitmapH;
			int32_t	OriginX;
			int32_t OriginY;
			int16_t PosIncX;
			int16_t PosIncY;
		};

	private:
		Metrics Dimensions;
		std::unique_ptr<uint8_t[]> PixData;

	public:
		Glyph(const Font& font, char32_t char_id, bool gray = true, mat2 t = mat2::identity) noexcept;
		Glyph(Glyph&& g) noexcept;
		Glyph& operator=(Glyph&& g) noexcept;
		Glyph(const Glyph& g);
		Glyph& operator=(const Glyph& g);
		~Glyph();

		int32_t Size() const;
		int32_t BitmapW() const;
		int32_t BitmapH() const;
		int32_t OriginX() const;
		int32_t OriginY() const;
		int16_t PosIncX() const;
		int16_t PosIncY() const;
		Metrics GetMetrics() const;
		operator const uint8_t*() const;

		static int32_t Size(const Font& f, char32_t char_id, bool gray = true, mat2 t = mat2::identity);
	};
}
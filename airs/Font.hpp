#pragma once
#include "math.hpp"
#include <memory>
#include <cstdint>
#include <string>



namespace airs
{
	class Glyph
	{
		friend class Font;
	public:
		struct Metrics
		{
			std::int32_t BitmapW;
			std::int32_t BitmapH;
			std::int32_t OriginX;
			std::int32_t OriginY;
			std::int16_t PosIncX;
			std::int16_t PosIncY;
		};

	private:
		Metrics Dimensions;
		std::unique_ptr<uint8_t[]> PixData;

		Glyph(std::unique_ptr<uint8_t[]>& data, const Metrics& dimensions);

	public:
		Glyph(Glyph&& g) noexcept;
		Glyph& operator=(Glyph&& g) noexcept;
		Glyph(const Glyph& g);
		Glyph& operator=(const Glyph& g);
		~Glyph();

		std::int32_t Size() const;
		std::int32_t BitmapW() const;
		std::int32_t BitmapH() const;
		std::int32_t OriginX() const;
		std::int32_t OriginY() const;
		std::int16_t PosIncX() const;
		std::int16_t PosIncY() const;
		Metrics GetMetrics() const;
		operator const std::uint8_t* () const;
	};

	class Font
	{
	private:
		void* Handle;
		struct Metrics
		{
			std::int32_t Height;
			std::int32_t Ascent;
			std::int32_t Descent;
			std::int32_t InternalLeading;
			std::int32_t ExternalLeading;
			std::int32_t AveCharWidth;
			std::int32_t MaxCharWidth;
			std::int32_t Weight;
			std::int32_t Overhang;
			std::int32_t DigitizedAspectX;
			std::int32_t DigitizedAspectY;
			char16_t FirstChar;
			char16_t LastChar;
			char16_t DefaultChar;
			char16_t BreakChar;
			std::uint8_t Italic;
			std::uint8_t Underlined;
			std::uint8_t StruckOut;
			std::uint8_t PitchAndFamily;
			std::uint8_t CharSet;
		} FontMetrics;

	public:
		Font(void* data, size_t lenght, const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(const std::string& file, const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(const std::string& font, int32_t size, float escapement, float orientation, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(const std::string& file, const std::string& font, int32_t size, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(const std::string& font, int32_t size, int32_t weight, bool italic, bool underline, bool strikeout, uint32_t pitchandfamily);
		Font(const std::string& file, const std::string& font, int32_t size);
		Font(const std::string& font, int32_t size);
		Font(const std::string& font);
		Font(int32_t size);
		Font();
		Font(Font&& font) noexcept;
		Font& operator=(Font&& font) noexcept;
		Font(const Font&) = delete;
		~Font();
		Font& operator=(const Font&) = delete;

		std::int32_t Height() const;
		std::int32_t Ascent() const;
		std::int32_t Descent() const;
		std::int32_t InternalLeading() const;
		std::int32_t ExternalLeading() const;
		std::int32_t AveCharWidth() const;
		std::int32_t MaxCharWidth() const;
		std::int32_t Weight() const;
		std::int32_t Overhang() const;
		std::int32_t DigitizedAspectX() const;
		std::int32_t DigitizedAspectY() const;
		char16_t FirstChar() const;
		char16_t LastChar() const;
		char16_t DefaultChar() const;
		char16_t BreakChar() const;
		std::uint8_t Italic() const;
		std::uint8_t Underlined() const;
		std::uint8_t StruckOut() const;
		std::uint8_t PitchAndFamily() const;
		std::uint8_t CharSet() const;

		enum Weight : std::int32_t
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
		enum Pitch : std::uint32_t
		{
			Default =		0, 
			Fixed =			1,			
			Variable =		2, 
		};
		enum Family : std::uint32_t
		{
			FDontCare =		0 << 4, 
			Roman =			1 << 4, 
			Swiss =			2 << 4, 
			Modern =		3 << 4, 
			Script =		4 << 4, 
			Decorative =	5 << 4, 
		};

		std::int32_t Size(char32_t char_id, bool gray = true, mat2f t = mat2f::identity) const;
		Glyph GetGlyph(char32_t char_id, bool gray = true, mat2f t = mat2f::identity) const;
		Glyph operator[](char32_t char_id) const { return GetGlyph(char_id); }
	};
}
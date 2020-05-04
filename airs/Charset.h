#pragma once
#include <unordered_map>
#include "Font.h"
#include "Bindable.h"
#include "TextureBuffer.h"
#include "Shader.h"



namespace airs
{

	class Charset : public Bindable
	{
	public:
		struct Metrics
		{
			Glyph::Metrics Base;
			int32_t Index;
		};

		Charset(const Charset&) = delete;
		Charset& operator=(const Charset&) = delete;
		Charset();

		virtual int32_t Height() const = 0;
		virtual size_t Size() const = 0;

		virtual bool Valid(char32_t id) const = 0;
		virtual bool Load(char32_t id) = 0;
		virtual Metrics FindLoad(char32_t id) = 0;
		virtual const Metrics& operator[](char32_t id) const = 0;
	};


	class StaticCharset : public Charset
	{
	protected:
		int32_t CharsHeight;
		char32_t RangeOrigin, RangeLength;
		std::unique_ptr<Metrics[]> GlyphSet;
		TextureBuffer CharsTBO;

	public:
		StaticCharset(StaticCharset&& cs);
		StaticCharset(const StaticCharset&) = delete;
		StaticCharset& operator=(const StaticCharset&) = delete;
		StaticCharset(const Font& font, char32_t begin = 0, char32_t length = 128, bool gray = true);
		~StaticCharset();

		virtual int32_t Height() const override;
		virtual size_t Size() const override;
		virtual bool Valid(char32_t id) const override;
		virtual bool Load(char32_t id) override;
		virtual Metrics FindLoad(char32_t id) override;
		virtual const Metrics& operator[](char32_t id) const override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};


	class DynamicCharset : public Charset
	{
	private:
		Font SourceFont;
		bool GrayScale;

		size_t TextureSize;
		size_t TextureCursor;
		
		TextureBuffer TextureMap;
		std::unordered_map<char32_t, Metrics> MetricsSet;

		bool ExtendCapacity();
		std::pair<bool, Metrics> LoadGlyph(char32_t id);

	public:
		DynamicCharset(DynamicCharset&& cs);
		DynamicCharset(const DynamicCharset&) = delete;
		DynamicCharset& operator=(const DynamicCharset&) = delete;
		DynamicCharset(Font&& font, size_t capacity, bool gray = false);
		DynamicCharset(Font&& font, bool gray = false);
		~DynamicCharset();

		Font& GetFont();

		virtual int32_t Height() const override;
		virtual size_t Size() const override;
		virtual bool Valid(char32_t id) const override;
		virtual bool Load(char32_t id) override;
		virtual Metrics FindLoad(char32_t id) override;
		virtual const Metrics& operator[](char32_t id) const override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}
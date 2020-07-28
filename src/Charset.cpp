#include "airs/Charset.hpp"



namespace airs
{
	/*Charset::Charset() 
	{
	}

	StaticCharset::StaticCharset(StaticCharset&& cs) : GlyphSet(std::move(cs.GlyphSet)), CharsTBO(std::move(cs.CharsTBO))
	{
		std::swap(CharsHeight, cs.CharsHeight);
		std::swap(RangeOrigin, cs.RangeOrigin);
		std::swap(RangeLength, cs.RangeLength);
	}
	StaticCharset::StaticCharset(const Font& font, char32_t begin, char32_t length, bool gray) : CharsTBO(TextureBuffer::R8UI)
	{
		CharsHeight = font.Height();
		RangeOrigin = begin;
		RangeLength = length;
		GlyphSet = std::make_unique<Charset::Metrics[]>(RangeLength);

		int32_t capacity = 0;
		for (uint32_t i = begin; i < begin + length; i++)
			capacity += Glyph::Size(font, i) + sizeof(int32_t) * 2;

		uint8_t* data = new uint8_t[capacity];
		int32_t cursor = 0;

		for (char32_t i = begin; i < begin + length; i++)
		{
			Glyph glyph(font, i, gray);

			GlyphSet[static_cast<size_t>(i) - static_cast<size_t>(begin)] = { glyph.GetMetrics(), cursor };

			std::memcpy(data + cursor, glyph - sizeof(int32_t) * 2ull, glyph.Size() + sizeof(int32_t) * 2ull);
			cursor += glyph.Size() + sizeof(int32_t) * 2ull;
		}

		CharsTBO.Data(capacity, data, Buffer::Usage::StaticDraw);

		delete[] data;
	}
	StaticCharset::~StaticCharset()
	{
	}

	int32_t StaticCharset::Height() const
	{
		return CharsHeight;
	}
	size_t StaticCharset::Size() const
	{
		return RangeLength;
	}
	bool StaticCharset::Valid(char32_t id) const
	{
		return id >= RangeOrigin && id < RangeOrigin + RangeLength;
	}
	bool StaticCharset::Load(char32_t id)
	{
		return false;
	}
	Charset::Metrics StaticCharset::FindLoad(char32_t id)
	{
		if (id >= RangeOrigin && id < RangeOrigin + RangeLength) return GlyphSet[static_cast<size_t>(id) - static_cast<size_t>(RangeOrigin)];
		else return Charset::Metrics{ Glyph::Metrics{ 0 }, -1 };
	}
	const Charset::Metrics& StaticCharset::operator[](char32_t id) const
	{
		return GlyphSet[static_cast<size_t>(id) - static_cast<size_t>(RangeOrigin)];
	}

	void StaticCharset::Bind() const
	{
		CharsTBO.Bind();
	}
	void StaticCharset::Unbind() const
	{
		CharsTBO.Unbind();
	}

	bool DynamicCharset::ExtendCapacity()
	{
		static const size_t max_capacity = TextureBuffer::MaxSize();
		if (TextureSize == max_capacity) return false;
		if ((TextureSize *= 2ull) > max_capacity) TextureSize = max_capacity;

		uint8_t* temp = new uint8_t[TextureCursor];
		TextureMap.GetSubData(0, TextureCursor, temp);
		TextureMap.Data(TextureSize, nullptr, Buffer::Usage::StaticDraw);
		TextureMap.SubData(0, TextureCursor, temp);
		delete[] temp;

		return true;
	}
	std::pair<bool, Charset::Metrics> DynamicCharset::LoadGlyph(char32_t id)
	{
		Glyph glyph(SourceFont, id, GrayScale);
		while (TextureCursor + glyph.Size() + sizeof(int32_t) * 2ull > TextureSize) 
			if (!ExtendCapacity()) return { false, { { 0 }, -1 } };

		TextureMap.SubData(TextureCursor, glyph.Size() + sizeof(int32_t) * 2, glyph - sizeof(int32_t) * 2);

		size_t cursor = TextureCursor;
		TextureCursor += glyph.Size() + sizeof(int32_t) * 2ull;
		return { true, { glyph.GetMetrics(), static_cast<int32_t>(cursor) } };
	}

	DynamicCharset::DynamicCharset(DynamicCharset&& cs) : MetricsSet(std::move(cs.MetricsSet)), 
		TextureMap(std::move(cs.TextureMap)), SourceFont(std::move(cs.SourceFont))
	{
		std::swap(GrayScale, cs.GrayScale);
		std::swap(TextureSize, cs.TextureSize);
		std::swap(TextureCursor, cs.TextureCursor);
	}
	DynamicCharset::DynamicCharset(Font&& font, size_t capacity, bool gray) : SourceFont(std::move(font)),
		TextureMap(capacity, Buffer::Usage::StaticDraw, TextureBuffer::R8UI), TextureSize(capacity), GrayScale(gray),
		TextureCursor(0)
	{
		
	}
	DynamicCharset::DynamicCharset(Font&& font, bool gray) : DynamicCharset(std::move(font), 1024, gray)
	{
	}
	DynamicCharset::~DynamicCharset()
	{
	}

	Font& DynamicCharset::GetFont()
	{
		return SourceFont;
	}

	int32_t DynamicCharset::Height() const
	{
		return SourceFont.Height();
	}
	size_t DynamicCharset::Size() const
	{
		return MetricsSet.size();
	}

	bool DynamicCharset::Valid(char32_t id) const
	{
		return MetricsSet.find(id) != MetricsSet.end();
	}
	bool DynamicCharset::Load(char32_t id)
	{
		if (Valid(id)) return true;
		auto data = LoadGlyph(id);
		if (!data.first) return false;
		MetricsSet.insert(std::make_pair(id, data.second));
		return true;
	}
	Charset::Metrics DynamicCharset::FindLoad(char32_t id)
	{
		auto it = MetricsSet.find(id);
		if (it != MetricsSet.end()) return it->second;
		else return MetricsSet.emplace(id, LoadGlyph(id).second).first->second;
	}
	const Charset::Metrics& DynamicCharset::operator[](char32_t id) const
	{
		return MetricsSet.at(id);
	}

	void DynamicCharset::Bind() const
	{
		TextureMap.Bind();
	}
	void DynamicCharset::Unbind() const
	{
		TextureMap.Unbind();
	}*/
}
#include "airs/Text.h"
#include "airs/Initializer.h"
#include "airs/Utilities.h"
#include "gl/glew.h"



namespace airs
{
	void Text::Extend()
	{
		if (RenderData) TextVBO.Unmap();
		RenderData = (Vertex*)TextVBO.Map(Buffer::Access::ReadOnly);
		Vertex* temp = new Vertex[TextLength];
		memcpy(temp, RenderData, sizeof(Vertex) * TextLength);
		TextVBO.Unmap();

		size_t new_length = TextLength <= 0 ? 16 : TextLength * 2;
		TextVBO.Data(sizeof(Vertex) * new_length, nullptr, Buffer::Usage::DynamicDraw);

		RenderData = (Vertex*)TextVBO.Map(Buffer::Access::WriteOnly);
		memcpy(RenderData, temp, sizeof(Vertex) * TextLength);
		delete[] temp;

		TextCapacity = new_length;
	}
	void Text::SetChar(size_t i, int32_t slot, int32_t index, vec2f pos, uint32_t color)
	{
		while (i >= TextCapacity) Extend();

		if (!RenderData) RenderData = (Vertex*)TextVBO.Map(Buffer::Access::WriteOnly);

		RenderData[i] = { { slot, index }, pos, color };
	}
	void Text::PushChar(int32_t slot, int32_t index, vec2f pos, uint32_t color)
	{
		if (TextLength == TextCapacity) Extend();

		if (!RenderData) RenderData = (Vertex*)TextVBO.Map(Buffer::Access::WriteOnly);

		RenderData[TextLength] = { { slot, index }, pos, color };
		TextLength++;
	}

	Text::Text(Charset& charset, size_t capacity) : TextVBO(Buffer::Target::Array), CharsPool(charset), CurrentColor(0xffffffff)
	{
		RenderData = nullptr;

		TextCapacity = 0;
		TextLength = 0;

		TextVAO.AttribIPointer(TextVBO, 0, 2, VertexArray::Int, sizeof(Vertex), nullptr);
		TextVAO.AttribPointer(TextVBO, 1, 2, VertexArray::Float, false, sizeof(Vertex), (void*)8);
		TextVAO.AttribIPointer(TextVBO, 2, 1, VertexArray::UInt, sizeof(Vertex), (void*)16);

		Reserve(capacity);
	}
	Text::Text(Charset& charset) : Text(charset, 16)
	{
	}
	Text::~Text()
	{
	}

	void Text::Print(const std::u32string& text)
	{
		for (char32_t id : text) Print(id);
	}
	void Text::Print(const std::u16string& text)
	{
		Print(to_utf32(text));
	}
	void Text::Print(const std::string& text)
	{
		Print(to_utf32(text));
	}
	void Text::Print(const char32_t* text)
	{
		Print(std::u32string(text));
	}
	void Text::Print(const char16_t* text)
	{
		Print(to_utf32(text));
	}
	void Text::Print(const wchar_t* text)
	{
		Print(to_utf32(text));
	}
	void Text::Print(const char* text)
	{
		Print(to_utf32(text));
	}
	Text& Text::operator<<(const std::u32string& text)
	{
		for (char32_t id : text) Print(id);
		return *this;
	}
	Text& Text::operator<<(const std::u16string& text)
	{
		Print(to_utf32(text));
		return *this;
	}
	Text& Text::operator<<(const std::string& text)
	{
		Print(to_utf32(text));
		return *this;
	}
	Text& Text::operator<<(const char32_t* text)
	{
		Print(std::u32string(text));
		return *this;
	}
	Text& Text::operator<<(const char16_t* text)
	{
		Print(to_utf32(text));
		return *this;
	}
	Text& Text::operator<<(const wchar_t* text)
	{
		Print(to_utf32(text));
		return *this;
	}
	Text& Text::operator<<(const char* text)
	{
		Print(to_utf32(text));
		return *this;
	}

	void Text::Camera(float x, float y, float w, float h)
	{
		TextCamera = { x, y, w / 2.0f, h / 2.0f };
	}
	void Text::Camera(vec4f camera)
	{
		TextCamera = { camera.x, camera.y, camera.z / 2.0f, camera.w / 2.0f };
	}
	void Text::Position(float x, float y)
	{
		TextCamera.x = x;
		TextCamera.y = y;
	}
	void Text::Position(vec2f pos)
	{
		TextCamera.x = pos.x;
		TextCamera.y = pos.y;
	}
	void Text::Resolution(float w, float h)
	{
		TextCamera.z = w / 2.0f;
		TextCamera.w = h / 2.0f;
	}
	void Text::Resolution(vec2f size)
	{
		TextCamera.z = size.x / 2.0f;
		TextCamera.w = size.y / 2.0f;
	}
	void Text::Color(uint32_t color)
	{
		CurrentColor = color;
	}
	void Text::Render()
	{
		static Shader& TextShader = GraphicsInitializer::TextShader();
		static int32_t CameraUID = GraphicsInitializer::TextCameraUID();

		if (RenderData)
		{
			TextVBO.Unmap();
			RenderData = nullptr;
		}

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		TextShader.Bind();
		TextShader.FastUniform(CameraUID, TextCamera);
		CharsPool.Bind();
		TextVAO.Render(VertexArray::Points, TextLength);
		CharsPool.Unbind();
		TextShader.Unbind();
	}

	void Text::Reserve(size_t capacity)
	{
		if (capacity <= 0) return;
		TextVBO.Data(sizeof(Vertex) * capacity, nullptr, Buffer::Usage::DynamicDraw);
		TextCapacity = capacity;
		TextLength = 0;
	}
	void Text::Clear()
	{
		TextLength = 0;
	}

	size_t Text::Capacity() const
	{
		return TextCapacity;
	}
	size_t Text::Length() const
	{
		return TextLength;
	}


	StaticText::StaticText(Charset& charset, int32_t capacity) : Text(charset, capacity)
	{
		BaseX = 0.0f;
		BaseY = 0.0f;
		CursX = 0.0f;
		CursY = 0.0f;
	}
	StaticText::StaticText(Charset& charset) : Text(charset)
	{
		BaseX = 0.0f;
		BaseY = 0.0f;
		CursX = 0.0f;
		CursY = 0.0f;
	}
	StaticText::~StaticText()
	{
	}

	void StaticText::Clear()
	{
		 CursX = BaseX;
		 CursY = BaseY;
		 Text::Clear();
	}

	void StaticText::Origin(float x, float y)
	{
		BaseX = CursX = x;
		BaseY = CursY = y;
	}

	void StaticText::Print(char32_t id)
	{
		if (id == '\n')
		{
			CursX = BaseX;
			CursY -= CharsPool.Height();
		}
		else if (id == '\r') CursX = BaseX;
		else if (id == '\b') { if (TextLength > 0) TextLength--; }
		else if (CharsPool.Load(id))
		{
			auto& metrics = CharsPool[id];
			PushChar(0, metrics.Index, { CursX + metrics.Base.OriginX, CursY + metrics.Base.OriginY }, CurrentColor);
			CursX += metrics.Base.PosIncX;
			CursY += metrics.Base.PosIncY;
		}
	}


	void EditableText::Rebuild(int64_t p)
	{
		if (p < 0) p = 0;
		if (!Story.empty())
		{
			Story[0].px = 0.0f;
			Story[0].py = 0.0f;
		}
		for (size_t i = p; i < Story.size(); i++)
		{
			auto& item = Story[i];
			Charset::Metrics metrics = { { 0 }, -1 };
			switch (item.id)
			{
			case '\n':
				if (i + 1 < Story.size())
				{
					Story[i + 1].px = 0.0f;
					Story[i + 1].py = item.py - CharsPool.Height();
				}
				break;

			case '\r':
				if (i + 1 < Story.size())
				{
					Story[i + 1].px = 0.0f;
				}
				break;

			default:
				metrics = CharsPool.FindLoad(item.id);
				if (i + 1 < Story.size())
				{
					Story[i + 1].px = item.px + metrics.Base.PosIncX;
					Story[i + 1].py = item.py + metrics.Base.PosIncY;
				}
				break;
			}

			SetChar(i, 0, metrics.Index, { item.px + metrics.Base.OriginX, item.py + metrics.Base.OriginY }, item.color);
		}
		TextLength = Story.size();
	}

	EditableText::EditableText(Charset& charset, int32_t capacity) : Text(charset, capacity)
	{
		StoryPt = -1;
	}
	EditableText::EditableText(Charset& charset) : Text(charset)
	{
		StoryPt = -1;
	}
	EditableText::~EditableText()
	{
	}
	
	void EditableText::Print(char32_t id)
	{
		switch (id)
		{
		case '\b':
			if (StoryPt >= 0)
			{
				Story.erase(Story.begin() + StoryPt);
				Rebuild(StoryPt - 1);
				StoryPt--;
			}
			break;
		
		default:
			StoryPt++;
			Story.insert(Story.begin() + StoryPt, { 0.0f, 0.0f, id, CurrentColor });
			Rebuild(StoryPt - 1);
			break;
		}
	}
	void EditableText::Clear()
	{
		StoryPt = -1;
		Story.clear();
		Text::Clear();
	}
	void EditableText::Seek(int64_t i)
	{
		StoryPt += i;
		if (StoryPt < -1) StoryPt = -1;
		if (StoryPt >= static_cast<int64_t>(Story.size())) StoryPt = static_cast<int64_t>(Story.size()) - 1;
	}
	vec2f EditableText::Cursor() const
	{
		if (StoryPt < 0) return vec2<float>(0.0f, 0.0f);
		auto& item = Story[StoryPt];
		switch (item.id)
		{
		case '\n': return vec2f(0.0f, item.py - CharsPool.Height());
		case '\r': return vec2f(0.0f, item.py);
		default: return vec2f(item.px + CharsPool.FindLoad(item.id).Base.PosIncX,
			item.py + CharsPool.FindLoad(item.id).Base.PosIncY);
		}
	}
	float EditableText::LineHeight() const
	{
		return CharsPool.Height();
	}


	SimpleText::SimpleText(Font&& font, size_t capacity) : TextCharset(std::move(font)), EditableText(TextCharset, capacity)
	{
	}
	SimpleText::SimpleText(Font&& font) : TextCharset(std::move(font)), EditableText(TextCharset)
	{
	}
	Font& SimpleText::GetFont()
	{
		return TextCharset.GetFont();
	}
	DynamicCharset& SimpleText::GetCharset()
	{
		return TextCharset;
	}
}
#pragma once
#include "Charset.h"
#include "VertexArray.h"
#include <vector>



namespace airs
{	
	class Text
	{
	private:
		struct Vertex
		{
			vec2i texture;
			vec2f pos;
			uint32_t color;
		};

		Vertex* RenderData;

		VertexArray TextVAO;
		Buffer TextVBO;
		size_t TextCapacity;

		void Extend();

	protected:
		size_t TextLength;
		vec4f TextCamera;
		uint32_t CurrentColor;

		void SetChar(size_t i, int32_t slot, int32_t index, vec2f pos, uint32_t color);
		void PushChar(int32_t slot, int32_t index, vec2f pos, uint32_t color);

		Charset& CharsPool;

	public:
		Text(Charset& charset, size_t capacity);
		Text(Charset& charset);
		~Text();

		virtual void Print(char32_t id) = 0;
		void Print(const std::u32string& text);
		void Print(const std::u16string& text);
		void Print(const std::string& text);
		void Print(const char32_t* text);
		void Print(const char16_t* text);
		void Print(const wchar_t* text);
		void Print(const char* text);
		template<typename T>
		void Print(T data)
		{
			Print(std::to_string(data));
			return *this;
		}
		Text& operator<<(const std::u32string& text);
		Text& operator<<(const std::u16string& text);
		Text& operator<<(const std::string& text);
		Text& operator<<(const char32_t* text);
		Text& operator<<(const char16_t* text);
		Text& operator<<(const wchar_t* text);
		Text& operator<<(const char* text);
		template<typename T>
		Text& operator<<(const T& data)
		{
			Print(std::to_string(data));
			return *this;
		}

		void Camera(float x, float y, float w, float h);
		void Camera(vec4f camera);
		void Position(float x, float y);
		void Position(vec2f pos);
		void Resolution(float w, float h);
		void Resolution(vec2f size);
		void Color(uint32_t color);
		virtual void Render();

		void Reserve(size_t capacity);
		virtual void Clear();

		size_t Capacity() const;
		size_t Length() const;
	};

	class StaticText : public Text
	{
	private:
		float BaseX, BaseY, CursX, CursY;

	public:
		StaticText(Charset& charset, int32_t capacity);
		StaticText(Charset& charset);
		~StaticText();

		virtual void Clear() override;

		void Origin(float x, float y);

		void Print(char32_t id) override;
	};

	class EditableText : public Text 
	{
	private:
		struct StoryItem
		{
			float px, py;
			char32_t id;
			uint32_t color;
		};
		int64_t StoryPt;
		std::vector<StoryItem> Story;

		void Rebuild(int64_t p);

	public: 
		EditableText(Charset& charset, int32_t capacity);
		EditableText(Charset& charset);
		~EditableText();

		virtual void Print(char32_t id) override;
		virtual void Clear() override;
		void Seek(int64_t i);
		vec2f Cursor() const;
		float LineHeight() const;
	};

	class SimpleText : public EditableText
	{
	private:
		DynamicCharset TextCharset;

	public:
		SimpleText(Font&& font, size_t capacity);
		SimpleText(Font&& font);

		Font& GetFont();
		DynamicCharset& GetCharset();
	};
}
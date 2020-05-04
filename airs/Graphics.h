#pragma once
#include <string>
#include "Spriteset.h"
#include "Charset.h"
#include "Buffer.h"
#include "math.h"



namespace airs
{
	struct GfxUnit
	{
		enum GfxMode : uint32_t
		{
			Empty,
			Flat,
			Image,
			Scaled,
			Symbol,
		};
		GfxMode Mode; 
		uint32_t ID;
		uint32_t Color;

		GfxUnit(GfxMode mode = Flat, uint32_t id = 0u, uint32_t color = 0xffffffffu) :
			Mode(mode), ID(id), Color(color)
		{

		}
		GfxUnit(uint32_t color) : Mode(Flat), ID(0), Color(color) { }
	};

	class Graphics
	{
	public:
		struct Vertex
		{
			vec2i Position;
			vec2i Size;
			uint32_t Mode;
			uint32_t Index;
			uint32_t Color;
		};

	private:
		const std::vector<uint32_t>& ImageIDs;
		Charset& Chars; 
		Buffer& SelectedBuffer;
		uint32_t& Capacity;
		uint32_t Cursor;
		bool Request;

		void Extend(uint32_t capacity);

	public:
		Graphics(const std::vector<uint32_t>& imgid, Charset& chars, Buffer& buffer, uint32_t& capacity);
		~Graphics();

		void RequestRefresh();
		bool RefreshRequested() const;

		void RenderEmpty();
		void RenderPane(vec2i pos, vec2i size, uint32_t color = 0xffffffffu);
		void RenderImage(vec2i pos, vec2i size, uint32_t id, uint32_t color = 0xffffffffu);
		void RenderScaled(vec2i pos, vec2i size, uint32_t id, uint32_t color = 0xffffffffu);
		void RenderString(vec2i pos, const std::u32string& str, uint32_t color = 0xffffffffu);
		vec2i RenderString(vec2i pos, uint32_t curs, const std::u32string& str, uint32_t color = 0xffffffffu);
		void Render(vec2i pos, vec2i size, GfxUnit unit);

		uint32_t LocateCursor(vec2i cur, vec2i pos, const std::u32string& str) const;
		vec2i MeasureString(const std::u32string& str) const;
		int32_t LineHeight() const;

		void Skip(uint32_t n);
		uint32_t GetCursor() const;
	};

	namespace test
	{
		class Graphics
		{
		private:
			vec2i Size, ClippedPosition, ClippedSize;
			uint32_t* Pixels;

			void Adjust(vec2i& pos, vec2i& size);

		public:
			Graphics(vec2i size);
			~Graphics();

			void Clip(vec2i pos, vec2i size);
			void Unclip();

			void DrawRect(vec2i pos, vec2i size, uint32_t color);
			void DrawBitmap(vec2i pos, vec2i size, vec2i offset, const Bitmap& bmp, uint32_t color);
			void DrawString(vec2i pos, vec2i size, const Font& font, const std::string& str, uint32_t color);

			uint32_t Pixel(vec2i pos) const;
			//uint32_t LocateCursor(vec2i cur, vec2i pos, const std::u32string& str) const;
			//vec2i MeasureString(const std::u32string& str) const;
			//int32_t LineHeight() const;
		};
	}
}


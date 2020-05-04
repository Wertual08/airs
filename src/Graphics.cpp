#include "airs/Graphics.h"
#include "airs/Utilities.h"
#include "AIRSWin.h"



namespace airs
{
	void Graphics::Extend(uint32_t capacity)
	{
		uint32_t from = Capacity;
		if (Capacity == 0) Capacity = capacity;
		else Capacity = std::max(Capacity * 2, capacity);
		SelectedBuffer.FastExtend(from * sizeof(Vertex), Capacity * sizeof(Vertex), Buffer::Usage::StaticDraw);
	}

	Graphics::Graphics(const std::vector<uint32_t>& imgid, Charset& chars, Buffer& buffer, uint32_t& capacity) :
		ImageIDs(imgid), Chars(chars), SelectedBuffer(buffer), Capacity(capacity), Cursor(0), Request(false)
	{
		SelectedBuffer.Bind();
	}
	Graphics::~Graphics()
	{
		SelectedBuffer.Unbind();
	}

	void Graphics::RequestRefresh()
	{
		Request = true;
	}
	bool Graphics::RefreshRequested() const
	{
		return Request;
	}

	void Graphics::RenderEmpty()
	{
		Vertex v{ 0, 0, GfxUnit::Empty, 0, 0 };
		SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
	}
	void Graphics::RenderPane(vec2i pos, vec2i size, uint32_t color)
	{
		if (Cursor >= Capacity) Extend(Cursor + 1);
		Vertex v = { pos, size, GfxUnit::Flat, 0, color };
		SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
	}
	void Graphics::RenderImage(vec2i pos, vec2i size, uint32_t id, uint32_t color)
	{
		if (id >= ImageIDs.size())
		{
			RenderPane(pos, size, color);
			return;
		}
		if (Cursor >= Capacity) Extend(Cursor + 1);
		Vertex v{ pos, size, GfxUnit::Image, static_cast<uint32_t>(ImageIDs[id]), color };
		SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
	}
	void Graphics::RenderScaled(vec2i pos, vec2i size, uint32_t id, uint32_t color)
	{
		if (id >= ImageIDs.size())
		{
			RenderPane(pos, size, color);
			return;
		}
		if (Cursor >= Capacity) Extend(Cursor + 1);
		Vertex v{ pos, size, GfxUnit::Scaled, static_cast<uint32_t>(ImageIDs[id]), color };
		SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
	}
	void Graphics::RenderString(vec2i pos, const std::u32string& str, uint32_t color)
	{
		Vertex v;
		for (char32_t c : str)
		{
			switch (c)
			{
			case '\r':
				pos.x = 0;
				v = { 0, 0, GfxUnit::Empty, 0, 0 };
				SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
				break;
			case '\n':
				pos.x = 0;
				pos.y -= Chars.Height();
				v = { 0, 0, GfxUnit::Empty, 0, 0 };
				SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
				break;

			default:
				if (Cursor >= Capacity) Extend(Cursor + 1);
				auto m = Chars.FindLoad(c);
				v = { pos + vec2i(m.Base.OriginX, m.Base.OriginY), vec2i(m.Base.BitmapW, m.Base.BitmapH),
					GfxUnit::Symbol, static_cast<uint32_t>(m.Index), color };
				SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
				pos.x += m.Base.PosIncX;
				pos.y += m.Base.PosIncY;
				break;
			}
		}
	}
	vec2i Graphics::RenderString(vec2i pos, uint32_t curs, const std::u32string& str, uint32_t color)
	{
		vec2i res = { pos.x, pos.y };
		Vertex v;
		for (char32_t c : str)
		{
			switch (c)
			{
			case '\r':
				pos.x = 0;
				v = { 0, 0, GfxUnit::Empty, 0, 0 };
				SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
				break;
			case '\n':
				pos.x = 0;
				pos.y -= Chars.Height();
				v = { 0, 0, GfxUnit::Empty, 0, 0 };
				SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
				break;

			default:
				if (Cursor >= Capacity) Extend(Cursor + 1);
				auto m = Chars.FindLoad(c);
				v = { pos + vec2i(m.Base.OriginX, m.Base.OriginY), vec2i(m.Base.BitmapW, m.Base.BitmapH),
					GfxUnit::Symbol, static_cast<uint32_t>(m.Index), color };
				SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
				pos.x += m.Base.PosIncX;
				pos.y += m.Base.PosIncY;
				break;
			}
			if (curs)
			{
				res = { pos.x, pos.y };
				curs--;
			}
		}
		return res;
	}
	void Graphics::Render(vec2i pos, vec2i size, GfxUnit unit)
	{
		if (Cursor >= Capacity) Extend(Cursor + 1);
		uint32_t index = 0;
		if (unit.Mode == GfxUnit::Image || unit.Mode == GfxUnit::Scaled)
		{
			if (unit.ID < ImageIDs.size()) index = ImageIDs[unit.ID];
			else unit.Mode = GfxUnit::Flat;
		}
		else if (unit.Mode == GfxUnit::Symbol) index = Chars.FindLoad(unit.ID).Index;
		Vertex v{ pos, size, unit.Mode, index, unit.Color };
		SelectedBuffer.FastSubData(Cursor++ * sizeof(Vertex), sizeof(Vertex), &v);
	}

	uint32_t Graphics::LocateCursor(vec2i cur, vec2i pos, const std::u32string& str) const
	{
		uint32_t height = pos.y;
		if (cur.y >= height + Chars.Height()) return 0;
		uint32_t index = 0;
		while (index < str.length() && height > cur.y)
		{
			if (str[index] == '\n') height -= Chars.Height();
			index++;
		}

		if (index == str.length()) return index;
		int32_t dist = abs(cur.x - pos.x);
		int32_t c = pos.x;
		for (int32_t i = index; i < str.length(); i++)
		{
			if (str[i] == '\n') return i;
			auto m = Chars.FindLoad(str[i]);
			c += m.Base.PosIncX;
			if (dist > abs(cur.x - c)) dist = abs(cur.x - c);
			else return i;
		}
		return str.length();;
	}
	vec2i Graphics::MeasureString(const std::u32string& str) const
	{
		vec2i res = vec2i(0, Chars.Height());
		for (char32_t c : str)
		{
			switch (c)
			{
			case '\r': break;
			case '\n': res.y += Chars.Height(); break;

			default:
				auto m = Chars.FindLoad(c);
				res.x += m.Base.PosIncX;
				res.y += m.Base.PosIncY;
				break;
			}
		}
		return res;
	}
	int32_t Graphics::LineHeight() const
	{
		return Chars.Height();
	}

	void Graphics::Skip(uint32_t n)
	{
		Cursor += n;
	}
	uint32_t Graphics::GetCursor() const
	{
		return Cursor;
	}

	namespace test
	{
		void Graphics::Adjust(vec2i& pos, vec2i& size)
		{
			if (pos.x < ClippedPosition.x)
			{
				size.x -= ClippedPosition.x - pos.x;
				pos.x = ClippedPosition.x;
			}
			if (pos.y < ClippedPosition.y)
			{
				size.y -= ClippedPosition.y - pos.y;
				pos.y = ClippedPosition.y;
			}
			if (size.x > ClippedSize.x) size.x = ClippedSize.x;
			if (size.y > ClippedSize.y) size.y = ClippedSize.y;
		}

		Graphics::Graphics(vec2i size)
		{
			Size = size;
			ClippedPosition = 0;
			ClippedSize = size;
			Pixels = new uint32_t[static_cast<size_t>(size.x) * static_cast<size_t>(size.y)];
			std::memset(Pixels, 0, static_cast<size_t>(size.x) * static_cast<size_t>(size.y) * sizeof(uint32_t));
		}
		Graphics::~Graphics()
		{
			delete[] Pixels;
		}

		void Graphics::Clip(vec2i pos, vec2i size)
		{
			ClippedPosition = pos;
			ClippedSize = { std::min(size.x, Size.x - pos.x), std::min(size.y, Size.y - pos.y) };
		}
		void Graphics::Unclip()
		{
			ClippedPosition = 0;
			ClippedSize = Size;
		}

		void Graphics::DrawRect(vec2i pos, vec2i size, uint32_t color)
		{
			Adjust(pos, size);
			for (int32_t y = 0; y < size.y; y++)
				for (int32_t x = 0; x < size.x; x++)
					Pixels[(pos.y + y) * Size.x + pos.x + x] = color;
		}
		void Graphics::DrawBitmap(vec2i pos, vec2i size, vec2i offset, const Bitmap& bmp, uint32_t color)
		{
			Adjust(pos, size);
			int32_t bw = static_cast<int32_t>(bmp.Width());
			int32_t bh = static_cast<int32_t>(bmp.Height());
			for (int32_t y = 0; y < size.y; y++)
				for (int32_t x = 0; x < size.x; x++)
					Pixels[(pos.y + y) * Size.x + pos.x + x] = bmp[(y + offset.y) % bh * bw + (x + offset.x) % bw];
		}
		void Graphics::DrawString(vec2i pos, vec2i size, const Font& font, const std::string& str, uint32_t color)
		{
			Adjust(pos, size);
			RECT rect;
			SetRect(&rect, pos.x, Size.y - pos.y - size.y, pos.x + size.x - 1, Size.y - pos.y - 1);
			uint32_t* pixels;

			HDC FontDC = CreateCompatibleDC(0);
			BITMAPINFO bmi = { { sizeof(BITMAPINFOHEADER), Size.x, Size.y, 1, 32, BI_RGB, 
				0x000000ff, 0xff000000, 0x00ff0000, 0x0000ff00 }, {0,0,0,0} };
			HBITMAP bmp = CreateDIBSection(0, &bmi, DIB_RGB_COLORS, (void**)&pixels, nullptr, 0);
			memcpy(pixels, Pixels, Size.x * Size.y * sizeof(uint32_t));

			SelectObject(FontDC, font);
			SelectObject(FontDC, bmp);
			SetBkMode(FontDC, TRANSPARENT);
			//SetBkColor(FontDC, 0xffffffu);
			SetTextColor(FontDC, 0x00ffffffu);
			DrawTextW(FontDC, (LPCWSTR)to_utf16(str).c_str(), -1, &rect, DT_LEFT | DT_TOP);

			memcpy(Pixels, pixels, Size.x * Size.y * sizeof(uint32_t));
			DeleteDC(FontDC);
			DeleteObject(bmp);
		}

		uint32_t Graphics::Pixel(vec2i pos) const
		{
			return Pixels[pos.y * Size.x + pos.x];
		}
	}
}
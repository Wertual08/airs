#include "airs/GUIWindow.h"
#include "airs/Initializer.h"
#include <ctime>



namespace airs
{
	void GUIWindow::OnResize(int32_t w, int32_t h)
	{
		GraphicsWindow::OnResize(w, h);
	}
	void GUIWindow::OnMouseDown(int32_t x, int32_t y, key k)
	{
		GraphicsWindow::OnMouseDown(x, y, k);

		vec2i pos = vec2i(x, y) / SurfaceScale;
		for (auto it = Layouts.begin(); it != Layouts.end(); it++)
		{
			if ((*it)->OnMouseDown(pos, k))
			{
				Layouts.splice(Layouts.begin(), Layouts, it);
				return;
			}
		}
	}
	void GUIWindow::OnMouseMove(int32_t x, int32_t y)
	{
		GraphicsWindow::OnMouseMove(x, y);

		vec2i pos = vec2i(x, y) / SurfaceScale;
		for (auto f : Layouts) if (f->OnMouseMove(pos)) return;
	}
	void GUIWindow::OnMouseUp(int32_t x, int32_t y, key k)
	{
		GraphicsWindow::OnMouseUp(x, y, k);

		vec2i pos = vec2i(x, y) / SurfaceScale;
		for (auto f : Layouts) if (f->OnMouseUp(pos, k)) return;
	}

	void GUIWindow::OnChar(char32_t c)
	{
		GraphicsWindow::OnChar(c);
		for (auto f : Layouts) if (f->OnChar(c)) return;
	}
	void GUIWindow::OnKeyDown(key k)
	{
		GraphicsWindow::OnKeyDown(k);
		for (auto f : Layouts) if (f->OnKeyDown(k)) return;
	}
	void GUIWindow::OnKeyUp(key k)
	{
		GraphicsWindow::OnKeyUp(k);
		for (auto f : Layouts) if (f->OnKeyUp(k)) return;
	}

	//GUIWindow::GUIWindow(const std::string& title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx, int32_t majv, int32_t minv, bool core) :
	//	GraphicsWindow(title, w, h, x, y, s, sx, majv, minv, core), Textures(TextureBuffer::R32UI), TexturesCapacity(0), ImageIDs(1, 0),
	//	Chars(Font(16), true), SurfaceScale(1)
	//{
	//}
	GUIWindow::GUIWindow(Charset& chars, const std::string& title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx, int32_t majv, int32_t minv, bool core) :
		GraphicsWindow(title, w, h, x, y, s, sx, majv, minv, core), Textures(TextureBuffer::R32UI), TexturesCapacity(0), ImageIDs(1, 0),
		Chars(chars), SurfaceScale(1)
	{
	}
	GUIWindow::~GUIWindow()
	{
	}

	void GUIWindow::LoadBitmap(const Bitmap& bmp, int32_t fcount, int32_t fdelay)
	{
		const uint32_t size = bmp.Size() * sizeof(uint32_t) + 4 * sizeof(uint32_t);
		uint32_t last = ImageIDs[ImageIDs.size() - 1];
		uint32_t offset = last * sizeof(uint32_t);
		Textures.BufferBind();
		if (offset + size > TexturesCapacity)
		{
			if (TexturesCapacity == 0) TexturesCapacity = offset + size;
			else TexturesCapacity = std::max(TexturesCapacity * 2, offset + size);
			Textures.FastExtend(offset, TexturesCapacity, Buffer::Usage::StaticDraw);
		}
		uint32_t texw = bmp.Width();
		uint32_t texh = bmp.Height() / fcount;
		Textures.FastSubData(offset, sizeof(uint32_t), &fcount);
		Textures.FastSubData(offset + sizeof(uint32_t), sizeof(uint32_t), &fdelay);
		Textures.FastSubData(offset + 2 * sizeof(uint32_t), sizeof(uint32_t), &texw);
		Textures.FastSubData(offset + 3 * sizeof(uint32_t), sizeof(uint32_t), &texh);
		Textures.FastSubData(offset + 4 * sizeof(uint32_t), bmp.Size() * sizeof(uint32_t), bmp);
		Textures.BufferUnbind();
		ImageIDs.push_back(last + 4 + bmp.Size());
	}

	void GUIWindow::Add(Layout& l)
	{
		l.Open();
		Layouts.push_back(&l);
	}

	void GUIWindow::Render() const
	{
		static Program& GUIShader = GraphicsInitializer::GUIShader();
		static int32_t ResolutionUID = GraphicsInitializer::GUIResolutionUID();
		static int32_t OffsetUID = GraphicsInitializer::GUIOffsetUID();
		static int32_t TimeUID = GraphicsInitializer::GUITimeUID();

		GraphicsWindow::DisableDepthTest();
		GraphicsWindow::EnableBlend();
		GraphicsWindow::BlendFunc();
		Texture::Active(0);
		Textures.Bind();
		Texture::Active(1);
		Chars.Bind();
		GUIShader.Bind(); 
		GUIShader.FastUniform(TimeUID, int32_t(clock() * 1000 / CLOCKS_PER_SEC % INT_MAX));
		GUIShader.FastUniform(ResolutionUID, Size / SurfaceScale);
		for (auto it = Layouts.begin(); it != Layouts.end(); it++)
		{
			auto& item = **it;
			if (item.Closed()) if ((it = Layouts.erase(it)) == Layouts.end()) break;
			GUIShader.FastUniform(OffsetUID, item.GetPostion());
			item.Render(ImageIDs, Chars);
		}
		GUIShader.Unbind();
		Chars.Unbind();
		Texture::Active(0);
		Textures.Unbind();
		GraphicsWindow::Render();
	}
}
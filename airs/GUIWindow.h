#pragma once
#include <memory>
#include <vector>
#include <list>
#include "GraphicsWindow.h"
#include "Layout.h"
#include "TextureBuffer.h"
#include "Charset.h"



namespace airs
{
	class GUIWindow : public GraphicsWindow
	{
	protected:
		std::vector<uint32_t> ImageIDs;
		uint32_t TexturesCapacity;
		TextureBuffer Textures;
		Charset& Chars;

		mutable std::list<Layout*> Layouts;

		int32_t SurfaceScale;

		virtual void OnResize(int32_t w, int32_t h) override;
		virtual void OnMouseDown(int32_t x, int32_t y, key k) override;
		virtual void OnMouseMove(int32_t x, int32_t y) override;
		virtual void OnMouseUp(int32_t x, int32_t y, key k) override;
		virtual void OnChar(char32_t c) override;
		virtual void OnKeyDown(key k) override;
		virtual void OnKeyUp(key k) override;

	public:
		//GUIWindow(const std::string& title, int32_t w, int32_t h, int32_t x = UseDefault,
		//	int32_t y = UseDefault, Style s = Style::Default, StyleEx sx = StyleEx::Default,
		//	int32_t majv = 3, int32_t minv = 3, bool core = false);
		GUIWindow(Charset& chars, const std::string& title, int32_t w, int32_t h, int32_t x = UseDefault,
			int32_t y = UseDefault, Style s = Style::Default, StyleEx sx = StyleEx::Default,
			int32_t majv = 3, int32_t minv = 3, bool core = false);
		~GUIWindow();

		void LoadBitmap(const Bitmap& bmp, int32_t fcount = 1, int32_t fdelay = 0);

		void Add(Layout& f);

		virtual void Render() const override;
	};
}

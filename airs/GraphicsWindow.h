#pragma once
#include "Window.h"
#include <string>



namespace airs
{
	class GraphicsWindow : public Window
	{
	private:
		int32_t ContextMajVersion;
		int32_t ContextMinVersion;
		bool ContextCoreProfile;

	protected:
		void* DeviceContext;
		void* GLContext;

		virtual void OnResize(int32_t w, int32_t h) override;


	public:
		static void ClearColor(float r, float g, float b, float a);
		static void ClearColor(vec4f color);
		static void Clear(bool color = true, bool depth = true);
		static void EnableBlend();
		static void DisableBlend();
		static void EnableDepthTest();
		static void DisableDepthTest();
		static void BlendFunc();

		GraphicsWindow(const std::string& title, int32_t w, int32_t h, int32_t x = UseDefault, 
			int32_t y = UseDefault, Style s = Style::Default, StyleEx sx = StyleEx::Default, 
			int32_t majv = 3, int32_t minv = 3, bool core = false);
		~GraphicsWindow();

		void Select() const;
		virtual void Render() const;
	};
}
#include "airs/GraphicsWindow.h"
#include "airs/Text.h"
#include "airs/SpritesRenderer.h"
#include "airs/Initializer.h"
#include "gl/glew.h"
#include "gl/wglew.h"



namespace airs
{
	inline void GraphicsWindow::OnResize(int32_t w, int32_t h)
	{
		wglMakeCurrent(static_cast<HDC>(DeviceContext), static_cast<HGLRC>(GLContext));
		glViewport(0, 0, w, h);
		Window::OnResize(w, h);
	}

	void GraphicsWindow::ClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
	void GraphicsWindow::ClearColor(vec4f color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void GraphicsWindow::Clear(bool color, bool depth)
	{
		glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
	}
	void GraphicsWindow::EnableBlend()
	{
		glEnable(GL_BLEND);
	}
	void GraphicsWindow::DisableBlend()
	{
		glDisable(GL_BLEND);
	}
	void GraphicsWindow::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}
	void GraphicsWindow::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}
	void GraphicsWindow::BlendFunc()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	GraphicsWindow::GraphicsWindow(const std::string& title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx, int32_t majv, int32_t minv, bool core) :
		DeviceContext(0), GLContext(0), ContextMajVersion(majv), ContextMinVersion(minv), ContextCoreProfile(core), Window(title, w, h, x, y, s, sx)
	{
		DeviceContext = GetDC(static_cast<HWND>(WindowHandle));
		static const int32_t PFAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_ALPHA_BITS_ARB, 8,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB, 4,
			0
		};

		int32_t PFID; UINT FormatsCount;
		if (!wglChoosePixelFormatARB(static_cast<HDC>(DeviceContext), PFAttribs, NULL, 1, &PFID, &FormatsCount) || FormatsCount == 0)
			throw std::runtime_error("airs::GraphicsWindow error : Choose pixel format failed.");
		PIXELFORMATDESCRIPTOR PFD;
		if (!DescribePixelFormat(static_cast<HDC>(DeviceContext), PFID, sizeof(PFD), &PFD))
			throw std::runtime_error("airs::GraphicsWindow error: Describe pixel format failed.");
		if (!SetPixelFormat(static_cast<HDC>(DeviceContext), PFID, &PFD))
			throw std::runtime_error("airs::GraphicsWindow error: Set pixel format failed.");

		int32_t GLCAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, ContextMajVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, ContextMinVersion,
			WGL_CONTEXT_PROFILE_MASK_ARB, ContextCoreProfile ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};
		
		if ((GLContext = wglCreateContextAttribsARB(static_cast<HDC>(DeviceContext), static_cast<HGLRC>(OpenGLInitializer::SharedContext()), GLCAttribs)) == 0)
			throw std::runtime_error("airs::GraphicsWindow error: OpenGL context creation failed.");
		if (!wglMakeCurrent(static_cast<HDC>(DeviceContext), static_cast<HGLRC>(GLContext)))
			throw std::runtime_error("airs::GraphicsWindow error: Making OpenGL context current failed.");
	}
	GraphicsWindow::~GraphicsWindow()
	{
		wglMakeCurrent(static_cast<HDC>(DeviceContext), NULL);
		wglDeleteContext(static_cast<HGLRC>(GLContext));
		ReleaseDC(static_cast<HWND>(WindowHandle), static_cast<HDC>(DeviceContext));
	}

	void GraphicsWindow::Select() const
	{
		wglMakeCurrent(static_cast<HDC>(DeviceContext), static_cast<HGLRC>(GLContext));
	}
	void GraphicsWindow::Render() const
	{
		SwapBuffers(static_cast<HDC>(DeviceContext));
	}
}
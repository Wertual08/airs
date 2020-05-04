#include <Windows.h>
#include "gl/glew.h"
#include "gl/wglew.h"
#include "airs/Initializer.h"
#include "airs/Window.h"
#include <gdiplus.h>
#include <iostream>

#pragma comment(lib, "gdiplus.lib")



namespace airs
{
	Initializer::Initializer()
	{
		WindowsInitializer::Init();
		OpenGLInitializer::Init();
		GraphicsInitializer::Init();
		GDIPlusInitializer::Init();
	}
	Initializer::~Initializer()
	{
		WindowsInitializer::Dispose();
		OpenGLInitializer::Dispose();
		GraphicsInitializer::Dispose();
		GDIPlusInitializer::Dispose();
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA))->OnMessage(hwnd, msg, wparam, lparam);
	}
	LRESULT CALLBACK Startup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (msg == WM_NCCREATE)
		{
			Window* p = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCT>(lparam)->lpCreateParams);
			if (p != nullptr)
			{
				SetWindowLongPtrW(static_cast<HWND>(hwnd), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc));
				SetWindowLongPtrW(static_cast<HWND>(hwnd), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p));
				return p->OnMessage(static_cast<HWND>(hwnd), msg, wparam, lparam);
			}
		}
		return DefWindowProcW(static_cast<HWND>(hwnd), msg, wparam, lparam);
	}
	bool WindowsInitialized = false;
	HINSTANCE Instance;
	LPCWSTR Name = L"AIRS_Framework_Window";
	ATOM Atom;
	void WindowsInitializer::Init()
	{
		Dispose();
		Instance = GetModuleHandleW(nullptr);
		WNDCLASSEXW wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = &Startup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = Instance;
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(0);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = Name;
		wc.hIconSm = nullptr;
		if ((Atom = RegisterClassExW(&wc)) == 0) throw std::runtime_error("airs::WindowsInitializer error: Can not register window class.");
		WindowsInitialized = true;
	}
	void WindowsInitializer::Dispose()
	{
		if (WindowsInitialized) UnregisterClassW(Name, Instance);
		WindowsInitialized = false;
	}
	bool WindowsInitializer::IsInitialized()
	{
		return WindowsInitialized;
	}
	void* WindowsInitializer::GetInstance()
	{
		if (!WindowsInitialized) throw std::runtime_error("airs::WindowsInitializer error: Window is not initialized.");
		return Instance;
	}
	LPCWSTR WindowsInitializer::GetName()
	{
		if (!WindowsInitialized) throw std::runtime_error("airs::WindowsInitializer error: Window is not initialized.");
		return Name;
	}
	ATOM WindowsInitializer::GetAtom()
	{
		if (!WindowsInitialized) throw std::runtime_error("airs::WindowsInitializer error: Window is not initialized.");
		return Atom;
	}


	bool OpenGLInitialized = false;
	HWND FakeHWND = nullptr;
	HDC FakeDC = nullptr;
	HGLRC SharedRC = nullptr;
	void OpenGLInitializer::Init()
	{
		Dispose();
		LPCWSTR cl = WindowsInitializer::GetName();
		HINSTANCE inst = static_cast<HINSTANCE>(WindowsInitializer::GetInstance());
		FakeHWND = CreateWindowExW(0, cl, 0, 0, 0, 0, 1, 1, 0, 0, inst, 0);
		if (FakeHWND == 0) throw std::runtime_error("airs::OpenGLInitializer error: Can not create fake window.");
		FakeDC = GetDC(FakeHWND);
		if (FakeDC == 0) throw std::runtime_error("airs::OpenGLInitializer error: Can not create fake device context.");
		PIXELFORMATDESCRIPTOR FakePFD =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};
		int FakePFDID = ChoosePixelFormat(FakeDC, &FakePFD);
		if (!FakePFDID) throw std::runtime_error("airs::OpenGLInitializer error: Can not choose fake pixel format.");
		if (!DescribePixelFormat(FakeDC, FakePFDID, sizeof(PIXELFORMATDESCRIPTOR), &FakePFD))
			throw std::runtime_error("airs::OpenGLInitializer error: Can not describe fake pixel format.");
		if (!SetPixelFormat(FakeDC, FakePFDID, &FakePFD))
			throw std::runtime_error("airs::OpenGLInitializer error: Can not set fake pixel format.");
		SharedRC = wglCreateContext(FakeDC);
		if (!SharedRC) throw std::runtime_error("airs::OpenGLInitializer error: Can not create fake GL context.");
		if (!wglMakeCurrent(FakeDC, SharedRC)) throw std::runtime_error("airs::OpenGLInitializer error: Can not make fake GL context current.");
		//glewExperimental = true;
		if (glewInit()) throw std::runtime_error("airs::OpenGLInitializer error: Can not initialize glew.");
		//GLint n = 0;
		//glGetIntegerv(GL_NUM_EXTENSIONS, &n);
		//for (GLint i = 0; i < n; i++)
		//{
		//	std::cout << (const char*)glGetStringi(GL_EXTENSIONS, i) << "\n";
		//}
		//int glVersion[2];
		//glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
		//glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
		//std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << "\n";
		//std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

		OpenGLInitialized = true;
	}
	void OpenGLInitializer::Dispose()
	{
		if (!OpenGLInitialized) return;

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(SharedRC);
		ReleaseDC(FakeHWND, FakeDC);
		DestroyWindow(FakeHWND);

		OpenGLInitialized = false;
	}
	bool OpenGLInitializer::IsInitialized()
	{
		return OpenGLInitialized;
	}
	void* OpenGLInitializer::SharedContext()
	{
		return SharedRC;
	}

	
	bool GraphicsInitialized = false;
	Shader* ResTextShader = nullptr; 
	int32_t ResTextCameraUID = -1;
	Shader* ResGUIShader = nullptr;
	int32_t ResGUIResolutionUID = -1;
	int32_t ResGUIOffsetUID = -1;
	int32_t ResGUITimeUID = -1;
	void InitTextShader()
	{
		ResTextShader = new Shader(R"{SHADER_SOURCE}(
#shader vertex
#version 330 core
layout(location = 0) in ivec2 InTexMetrics;
layout(location = 1) in vec2 InPosition;
layout(location = 2) in uint InColor;
uniform vec4 Camera = vec4(0.0f, 0.0f, 1.0f, 1.0f);
uniform isamplerBuffer TextureBuffer[gl_MaxGeometryTextureImageUnits];
flat out vec4 RectPosition;
flat out ivec4 TextureMetrics;
flat out vec4 VertexColor;
void main()
{
    TextureMetrics.xy = ivec2(InTexMetrics.x, InTexMetrics.y + 8);
    if (TextureMetrics.y < 8) return;
    ivec2 tsize = ivec2(
        (texelFetch(TextureBuffer[InTexMetrics.x], InTexMetrics.y + 0).r << 0) |
        (texelFetch(TextureBuffer[InTexMetrics.x], InTexMetrics.y + 1).r << 8) |
        (texelFetch(TextureBuffer[InTexMetrics.x], InTexMetrics.y + 2).r << 16) |
        (texelFetch(TextureBuffer[InTexMetrics.x], InTexMetrics.y + 3).r << 24),
        (texelFetch(TextureBuffer[InTexMetrics.x], InTexMetrics.y + 4).r << 0) |
        (texelFetch(TextureBuffer[InTexMetrics.x], InTexMetrics.y + 5).r << 8) |
        (texelFetch(TextureBuffer[InTexMetrics.x], InTexMetrics.y + 6).r << 16) |
        (texelFetch(TextureBuffer[InTexMetrics.x], InTexMetrics.y + 7).r << 24));
    RectPosition = (InPosition.xyxy + vec4(0.0f, 0.0f, tsize.x, tsize.y) + Camera.xyxy - Camera.zwzw) / Camera.zwzw;
    TextureMetrics.zw = tsize;
    VertexColor = vec4(InColor >> 24u, (InColor >> 16u) & 0xffu, (InColor >> 8u) & 0xffu, InColor & 0xffu) / 255.0f;
}

#shader geometry
#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
flat in vec4 RectPosition[1];
flat in ivec4 TextureMetrics[1];
flat in vec4 VertexColor[1];
flat out ivec4 TextureData;
out vec2 TexturePixel;
flat out vec4 FragmentColor;
void main()
{
    if (TextureMetrics[0].y < 8) return;
    FragmentColor = VertexColor[0];
    TextureData = TextureMetrics[0];
    gl_Position = vec4(RectPosition[0].zy, 0.0f, 1.0f);
    TexturePixel = vec2(TextureMetrics[0].z, 0.0f);
    EmitVertex();
    gl_Position = vec4(RectPosition[0].xy, 0.0f, 1.0f);
    TexturePixel = vec2(0.0f, 0.0f);
    EmitVertex();
    gl_Position = vec4(RectPosition[0].zw, 0.0f, 1.0f);
    TexturePixel = vec2(TextureMetrics[0].zw);
    EmitVertex();
    gl_Position = vec4(RectPosition[0].xw, 0.0f, 1.0f);
    TexturePixel = vec2(0, TextureMetrics[0].w);
    EmitVertex();
    EndPrimitive();
}

#shader fragment
#version 330 core
uniform isamplerBuffer TextureBuffer[gl_MaxGeometryTextureImageUnits];
flat in ivec4 TextureData;
in vec2 TexturePixel;
flat in vec4 FragmentColor;
out vec4 ResultColor;
void main()
{
    ivec2 pixel = ivec2(clamp(int(TexturePixel.x), 0, TextureData.z), clamp(int(TexturePixel.y), 0, TextureData.w));clamp(ivec2(TexturePixel), ivec2(0, 0), TextureData.yz + 10);
    float a = float(texelFetch(TextureBuffer[TextureData.x], TextureData.y + pixel.x + pixel.y * TextureData.z).r);
    if (a < 0.1f) discard;
    ResultColor = FragmentColor * vec4(1.0f, 1.0f, 1.0f, a / 255.0f);
}
		){SHADER_SOURCE}");

		ResTextCameraUID = ResTextShader->Uniform("Camera");
	}
	void InitGUIShader() 
	{
		ResGUIShader = new Shader(R"{SHADER_SOURCE}(
#shader vertex
#version 330 core

layout(location = 0) in ivec4 InMetrics;
layout(location = 1) in uvec3 InImgData;

flat out ivec4 Metrics;
flat out uint Mode;
flat out int Index;
flat out uint Color;

void main()
{
	Metrics = InMetrics;
	Mode = InImgData.x;
	Index = int(InImgData.y);
	Color = InImgData.z;
}


#shader geometry
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform ivec2 Resolution = ivec2(1, 1);
uniform ivec2 Offset = ivec2(0, 0);
uniform int Time = 0;
uniform usamplerBuffer TextureBuffer[2];

flat in ivec4 Metrics[1];
flat in uint Mode[1];
flat in int Index[1];
flat in uint Color[1];

flat out vec4 FragmentColor;
flat out uint TexMode;
flat out int TexIndex;
flat out ivec2 TexSize;
out vec2 TexturePixel;

void main()
{
	// Empty, Flat, Image, Scaled, Symbol,
	if (Mode[0] == 0u) return;
	
	vec2 Position = vec2(Metrics[0].xy + Offset) * 2.0f / vec2(Resolution) - 1.0f;
	vec2 Size = vec2(Metrics[0].zw) * 2.0f / vec2(Resolution);

	FragmentColor = vec4(Color[0] >> 24u, (Color[0] >> 16u) & 0xffu, (Color[0] >> 8u) & 0xffu, Color[0] & 0xffu) / 255.0f;
	TexMode = Mode[0];
	if (Mode[0] == 1u)
	{
		gl_Position = vec4(Position, 0.0f, 1.0f);
		EmitVertex();
		gl_Position = vec4(Position + vec2(Size.x, 0), 0.0f, 1.0f);
		EmitVertex();
		gl_Position = vec4(Position + vec2(0, Size.y), 0.0f, 1.0f);
		EmitVertex();
		gl_Position = vec4(Position + Size, 0.0f, 1.0f);
		EmitVertex();
		EndPrimitive();
	}
	else if (Mode[0] == 2u)
	{
		int FramesCount = int(texelFetch(TextureBuffer[0], Index[0]).r);
		int FrameDelay = int(texelFetch(TextureBuffer[0], Index[0] + 1).r);
		TexSize = ivec2(texelFetch(TextureBuffer[0], Index[0] + 2).r,
				texelFetch(TextureBuffer[0], Index[0] + 3).r);

		if (FrameDelay > 0) TexIndex = Index[0] + 4 + Time / FrameDelay % FramesCount * TexSize.x * TexSize.y;
		else if (FrameDelay < 0) TexIndex = Index[0] + 4 + (FramesCount - 1 + Time / FrameDelay % FramesCount) * TexSize.x * TexSize.y;
		else TexIndex = Index[0] + 4;

		TexturePixel = vec2(0);
		gl_Position = vec4(Position, 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(Metrics[0].z, 0);
		gl_Position = vec4(Position + vec2(Size.x, 0), 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(0, Metrics[0].w);
		gl_Position = vec4(Position + vec2(0, Size.y), 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(Metrics[0].z, Metrics[0].w);
		gl_Position = vec4(Position + Size, 0.0f, 1.0f);
		EmitVertex();
		EndPrimitive();
	}
	else if (Mode[0] == 3u)
	{
		int FramesCount = int(texelFetch(TextureBuffer[0], Index[0]).r);
		int FrameDelay = int(texelFetch(TextureBuffer[0], Index[0] + 1).r);

		TexSize = ivec2(texelFetch(TextureBuffer[0], Index[0] + 2).r,
				texelFetch(TextureBuffer[0], Index[0] + 3).r);

		if (FrameDelay > 0) TexIndex = Index[0] + 4 + Time / FrameDelay % FramesCount * TexSize.x * TexSize.y;
		else if (FrameDelay < 0) TexIndex = Index[0] + 4 + (FramesCount - 1 + Time / FrameDelay % FramesCount) * TexSize.x * TexSize.y;
		else TexIndex = Index[0] + 4;

		TexturePixel = vec2(0);
		gl_Position = vec4(Position, 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(TexSize.x, 0);
		gl_Position = vec4(Position + vec2(Size.x, 0), 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(0, TexSize.y);
		gl_Position = vec4(Position + vec2(0, Size.y), 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(TexSize);
		gl_Position = vec4(Position + Size, 0.0f, 1.0f);
		EmitVertex();
		EndPrimitive();
	}
	else if (Mode[0] == 4u)
	{
		TexIndex = Index[0] + 8;
		TexSize = ivec2(
			(texelFetch(TextureBuffer[1], Index[0] + 0).r << 0) |
			(texelFetch(TextureBuffer[1], Index[0] + 1).r << 8) |
			(texelFetch(TextureBuffer[1], Index[0] + 2).r << 16) |
			(texelFetch(TextureBuffer[1], Index[0] + 3).r << 24),
			(texelFetch(TextureBuffer[1], Index[0] + 4).r << 0) |
			(texelFetch(TextureBuffer[1], Index[0] + 5).r << 8) |
			(texelFetch(TextureBuffer[1], Index[0] + 6).r << 16) |
			(texelFetch(TextureBuffer[1], Index[0] + 7).r << 24));

		TexturePixel = vec2(0);
		gl_Position = vec4(Position, 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(Metrics[0].z, 0);
		gl_Position = vec4(Position + vec2(Size.x, 0), 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(0, Metrics[0].w);
		gl_Position = vec4(Position + vec2(0, Size.y), 0.0f, 1.0f);
		EmitVertex();
		TexturePixel = vec2(Metrics[0].z, Metrics[0].w);
		gl_Position = vec4(Position + vec2(Size.x, Size.y), 0.0f, 1.0f);
		EmitVertex();
		EndPrimitive();
	}
}


#shader fragment
#version 330 core

uniform usamplerBuffer TextureBuffer[2];

flat in vec4 FragmentColor;
flat in uint TexMode;
flat in int TexIndex;
flat in ivec2 TexSize;
in vec2 TexturePixel;

out vec4 Color;

void main()
{
	if (TexMode == 1u)
	{
		Color = FragmentColor;
	}
	else if (TexMode == 2u)
	{
		int x = int(TexturePixel.x) % TexSize.x;
		int y = int(TexturePixel.y) % TexSize.y;
		uint c = texelFetch(TextureBuffer[0], TexIndex + x + y * TexSize.x).r;
		Color = FragmentColor * vec4(c >> 24u, (c >> 16u) & 0xffu, (c >> 8u) & 0xffu, c & 0xffu) / 255.0f;
	}
	else if (TexMode == 3u)
	{
		int x = int(TexturePixel.x);
		int y = int(TexturePixel.y);
		uint c = texelFetch(TextureBuffer[0], TexIndex + x + y * TexSize.x).r;
		Color = FragmentColor * vec4(c >> 24u, (c >> 16u) & 0xffu, (c >> 8u) & 0xffu, c & 0xffu) / 255.0f;
	}
	else if (TexMode == 4u)
	{
		int x = int(TexturePixel.x);
		int y = int(TexturePixel.y);
		uint c = texelFetch(TextureBuffer[1], TexIndex + x + y * TexSize.x).r;
		Color = vec4(FragmentColor.rgb, FragmentColor.a * c / 255.0f);
	}
}
){SHADER_SOURCE}");
		int32_t ind = ResGUIShader->Uniform("TextureBuffer");
		ResGUIShader->Uniform(ind, 0);
		ResGUIShader->Uniform(ind + 1, 1);

		ResGUIResolutionUID = ResGUIShader->Uniform("Resolution");
		ResGUIOffsetUID = ResGUIShader->Uniform("Offset");
		ResGUITimeUID = ResGUIShader->Uniform("Time");
	}
	void GraphicsInitializer::Init()
	{
		Dispose();

		InitTextShader();
		InitGUIShader();

		GraphicsInitialized = true;
	}
	void GraphicsInitializer::Dispose()
	{
		if (!GraphicsInitialized) return;

		delete ResTextShader;
		delete ResGUIShader;

		GraphicsInitialized = false;
	}
	bool GraphicsInitializer::IsInitialized()
	{
		return GraphicsInitialized;
	}
	Shader& GraphicsInitializer::TextShader()
	{
		if (!GraphicsInitialized) throw std::runtime_error("airs::GraphicsInitializer error: Graphics is not initialized.");
		return *ResTextShader;
	}
	int32_t GraphicsInitializer::TextCameraUID()
	{
		if (!GraphicsInitialized) throw std::runtime_error("airs::GraphicsInitializer error: Graphics is not initialized.");
		return ResTextCameraUID;
	}
	Shader& GraphicsInitializer::GUIShader()
	{
		if (!GraphicsInitialized) throw std::runtime_error("airs::GraphicsInitializer error: Graphics is not initialized.");
		return *ResGUIShader;
	}
	int32_t GraphicsInitializer::GUIResolutionUID()
	{
		if (!GraphicsInitialized) throw std::runtime_error("airs::GraphicsInitializer error: Graphics is not initialized.");
		return ResGUIResolutionUID;
	}
	int32_t GraphicsInitializer::GUIOffsetUID()
	{
		if (!GraphicsInitialized) throw std::runtime_error("airs::GraphicsInitializer error: Graphics is not initialized.");
		return ResGUIOffsetUID;
	}
	int32_t GraphicsInitializer::GUITimeUID()
	{
		if (!GraphicsInitialized) throw std::runtime_error("airs::GraphicsInitializer error: Graphics is not initialized.");
		return ResGUITimeUID;
	}


	bool GDIPlusInitialized = false;
	uint64_t GDIPlusToken = 0;
	void GDIPlusInitializer::Init()
	{
		auto result = Gdiplus::GdiplusStartup((ULONG_PTR*)&GDIPlusToken, &Gdiplus::GdiplusStartupInput(), NULL);
		if (result != Gdiplus::Status::Ok) throw std::runtime_error("airs::GDIPlusInitializer error: Can not acquire GDI+ token.");

		GDIPlusInitialized = true;
	}
	void GDIPlusInitializer::Dispose()
	{
		if (GDIPlusInitialized) Gdiplus::GdiplusShutdown(GDIPlusToken);
		GDIPlusInitialized = false;
	}
	bool GDIPlusInitializer::IsInitialized()
	{
		return GDIPlusInitialized;
	}
	uint64_t GDIPlusInitializer::GetToken()
	{
		if (!GDIPlusInitialized) throw std::runtime_error("airs::GDIPlusInitializer error: GDIPlus is not initialized.");
		return GDIPlusToken;
	}
}
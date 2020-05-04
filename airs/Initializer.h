#pragma once
#include "Shader.h"



namespace airs
{
	class Initializer
	{
	public:
		Initializer();
		~Initializer();
	};

	class WindowsInitializer
	{
	public:
		WindowsInitializer() = default;
		static void Init();
		static void Dispose();
		static bool IsInitialized();
		static void* GetInstance();
		static const wchar_t* GetName();
		static uint16_t GetAtom();
	};

	class OpenGLInitializer
	{
	public:
		OpenGLInitializer() = default;
		static void Init();
		static void Dispose();
		static bool IsInitialized();
		static void* SharedContext();
	};

	class GraphicsInitializer
	{
	public:
		GraphicsInitializer() = default;
		static void Init();
		static void Dispose();
		static bool IsInitialized();
		static Shader& TextShader();
		static int32_t TextCameraUID();
		static Shader& GUIShader();
		static int32_t GUIResolutionUID();
		static int32_t GUIOffsetUID();
		static int32_t GUITimeUID();
	};

	class GDIPlusInitializer
	{
	public:
		GDIPlusInitializer() = default;
		static void Init();
		static void Dispose();
		static bool IsInitialized();
		static uint64_t GetToken();
	};
}
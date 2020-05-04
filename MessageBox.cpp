#include "airs/MessageBox.h"
#include "airs/Utilities.h"
#include <Windows.h>
#undef MessageBox



namespace airs
{
	MessageBox::Result MessageBox::Show(const std::string& message)
	{
		return static_cast<Result>(MessageBoxW(nullptr, (LPCWSTR)to_utf16(message).c_str(), L"\0", 0));
	}
	MessageBox::Result MessageBox::Show(const std::string& message, const std::string& caption)
	{
		return static_cast<Result>(MessageBoxW(nullptr, (LPCWSTR)to_utf16(message).c_str(), (LPCWSTR)to_utf16(caption).c_str(), 0));
	}
	MessageBox::Result MessageBox::Show(const std::string& message, uint32_t options)
	{
		return static_cast<Result>(MessageBoxW(nullptr, (LPCWSTR)to_utf16(message).c_str(), L"\0", options));
	}
	MessageBox::Result MessageBox::Show(const std::string& message, const std::string& caption, uint32_t options)
	{
		return static_cast<Result>(MessageBoxW(nullptr, (LPCWSTR)to_utf16(message).c_str(), (LPCWSTR)to_utf16(caption).c_str(), options));
	}
	MessageBox::Result MessageBox::Show(const Window& window, const std::string& message)
	{
		return static_cast<Result>(MessageBoxW(static_cast<HWND>(window.WindowHandle), (LPCWSTR)to_utf16(message).c_str(), L"\0", 0));
	}
	MessageBox::Result MessageBox::Show(const Window& window, const std::string& message, const std::string& caption)
	{
		return static_cast<Result>(MessageBoxW(static_cast<HWND>(window.WindowHandle), (LPCWSTR)to_utf16(message).c_str(), (LPCWSTR)to_utf16(caption).c_str(), 0));
	}
	MessageBox::Result MessageBox::Show(const Window& window, const std::string& message, uint32_t options)
	{
		return static_cast<Result>(MessageBoxW(static_cast<HWND>(window.WindowHandle), (LPCWSTR)to_utf16(message).c_str(), L"\0", options));
	}
	MessageBox::Result MessageBox::Show(const Window& window, const std::string& message, const std::string& caption, uint32_t options)
	{
		return static_cast<Result>(MessageBoxW(static_cast<HWND>(window.WindowHandle), (LPCWSTR)to_utf16(message).c_str(), (LPCWSTR)to_utf16(caption).c_str(), options));
	}
}
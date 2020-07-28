#include "airs/MessageBox.hpp"
#include "airs/Utilities.hpp"
#include <Windows.h>
#undef MessageBox



namespace airs
{
	MessageBox::Result MessageBox::Show(const std::string& message)
	{
		return static_cast<Result>(MessageBoxW(nullptr, to_wide(message).c_str(), L"\0", 0));
	}
	MessageBox::Result MessageBox::Show(const std::string& message, const std::string& caption)
	{
		return static_cast<Result>(MessageBoxW(nullptr, to_wide(message).c_str(), to_wide(caption).c_str(), 0));
	}
	MessageBox::Result MessageBox::Show(const std::string& message, uint32_t options)
	{
		return static_cast<Result>(MessageBoxW(nullptr, to_wide(message).c_str(), L"\0", options));
	}
	MessageBox::Result MessageBox::Show(const std::string& message, const std::string& caption, uint32_t options)
	{
		return static_cast<Result>(MessageBoxW(nullptr, to_wide(message).c_str(), to_wide(caption).c_str(), options));
	}
	MessageBox::Result MessageBox::Show(const Window& window, const std::string& message)
	{
		return static_cast<Result>(MessageBoxW(static_cast<HWND>(window.WindowHandle), to_wide(message).c_str(), L"\0", 0));
	}
	MessageBox::Result MessageBox::Show(const Window& window, const std::string& message, const std::string& caption)
	{
		return static_cast<Result>(MessageBoxW(static_cast<HWND>(window.WindowHandle), to_wide(message).c_str(), to_wide(caption).c_str(), 0));
	}
	MessageBox::Result MessageBox::Show(const Window& window, const std::string& message, uint32_t options)
	{
		return static_cast<Result>(MessageBoxW(static_cast<HWND>(window.WindowHandle), to_wide(message).c_str(), L"\0", options));
	}
	MessageBox::Result MessageBox::Show(const Window& window, const std::string& message, const std::string& caption, uint32_t options)
	{
		return static_cast<Result>(MessageBoxW(static_cast<HWND>(window.WindowHandle), to_wide(message).c_str(), to_wide(caption).c_str(), options));
	}
}
#include "airs/Window.h"
#include "airs/Initializer.h"
#include "airs/Utilities.h"
#include "AIRSWin.h"



namespace airs
{
	std::exception_ptr CurrentWindowException = nullptr;

	Window::UserInput::UserInput()
	{
		CharsLocked = false;
		CrearState();
	}
	void Window::UserInput::SetCursor(float x, float y)
	{
		CursorX = x;
		CursorY = y;
	}
	void Window::UserInput::OnMouseMove(float x, float y)
	{
		DeltaX += x - CursorX;
		DeltaY += y - CursorY;
		CursorX = x;
		CursorY = y;
	}
	void Window::UserInput::OnMouseWheel(float d)
	{
		Wheel += d;
	}
	void Window::UserInput::OnKeyDown(key b)
	{
		if (CharsLocked) return;
		int code = static_cast<int>(b);
		if (code < 0 || code >= 256) return;
		KeyStates[code] = true;
	}
	void Window::UserInput::OnKeyUp(key b)
	{
		if (CharsLocked) return;
		int code = static_cast<int>(b);
		if (code < 0 || code >= 256) return;
		KeyStates[code] = false;
	}
	bool Window::UserInput::OnChar(char32_t c)
	{
		if ((unsigned char)(RingWriter + 1) == RingReader) return false;
		else CharsRing[RingWriter++] = c;
		return true;
	}
	void Window::UserInput::CrearState()
	{
		CursorX = 0.0f;
		CursorY = 0.0f;
		DeltaX = 0.0f;
		DeltaY = 0.0f;
		Wheel = 0.0f;
		RingReader = 0;
		RingWriter = 0;
		KeyStates.reset();
	}
	Window::UserInput::~UserInput()
	{
	}

	void Window::UserInput::LockChars(bool flushkeystates)
	{
		if (flushkeystates) KeyStates.reset();
		CharsLocked = true;
	}
	void Window::UserInput::UnlockChars()
	{
		CharsLocked = false;
	}
	bool Window::UserInput::KeyState(key b) const
	{
		int code = static_cast<int>(b);
		if (code < 0 || code >= 256) return false;
		return KeyStates[code];
	}
	bool Window::UserInput::operator[](key b) const
	{
		int code = static_cast<int>(b);
		if (code < 0 || code >= 256) return false;
		return KeyStates[code];
	}
	float Window::UserInput::MouseX() const
	{
		return CursorX;
	}
	float Window::UserInput::MouseY() const
	{
		return CursorY;
	}
	vec2f Window::UserInput::Mouse() const
	{
		return vec2f(CursorX, CursorY);
	}
	float Window::UserInput::WheelDelta()
	{
		float temp = Wheel;
		Wheel = 0.0f;
		return temp;
	}
	float Window::UserInput::MouseDeltaX()
	{
		float temp = DeltaX;
		DeltaX = 0.0f;
		return temp;
	}
	float Window::UserInput::MouseDeltaY()
	{
		float temp = DeltaY;
		DeltaY = 0.0f;
		return temp;
	}
	char32_t Window::UserInput::GetChar()
	{
		if (RingReader == RingWriter) return 0;
		else return CharsRing[RingReader++];
	}
	void Window::UserInput::Clear()
	{
		RingReader = RingWriter;
	}

	int64_t Window::OnMessage(void* hwnd, uint32_t msg, uint64_t wparam, int64_t lparam)
	{
		try
		{
			switch (msg)
			{
			case WM_NCCREATE: WindowHandle = hwnd; break;
			case WM_NCCALCSIZE: break;
			case WM_CREATE: break;
			case WM_CLOSE: if (!OnClose()) return 1; else PostQuitMessage(0); break;
			case WM_SETCURSOR: {
				if (LOWORD(lparam) == HTCLIENT)
				{
					::SetCursor(static_cast<HCURSOR>(CursorHandle));
					return 1;
				}
			} break;
			case WM_SIZE: {
				RECT r;
				GetClientRect(static_cast<HWND>(WindowHandle), &r);
				Size = vec2i(r.right - r.left, r.bottom - r.top);
				OnResize(Size.x, Size.y);
			} break;
			case WM_KILLFOCUS: OnKillFocus(); break;

				//case (WM)WM_INPUTLANGCHANGE: return 0;  break;
				//case WM::Hotkey: return 1;//RegisterHotKey(hwnd, ); /*OnHotkey();*/ break;

			case WM_LBUTTONDOWN: {
				const POINTS pt = MAKEPOINTS(lparam);
				OnMouseDown(pt.x, Size.y - 1 - pt.y, key::LButton);
				OnKeyDown(key::LButton);
			} break; 
			case WM_MBUTTONDOWN: {
				const POINTS pt = MAKEPOINTS(lparam);
				OnMouseDown(pt.x, Size.y - 1 - pt.y, key::MButton);
				OnKeyDown(key::MButton);
			} break; 
			case WM_RBUTTONDOWN: {
				const POINTS pt = MAKEPOINTS(lparam);
				OnMouseDown(pt.x, Size.y - 1 - pt.y, key::RButton);
				OnKeyDown(key::RButton);
			} break;
			case WM_XBUTTONDOWN: {
				const POINTS pt = MAKEPOINTS(lparam);
				if (GET_XBUTTON_WPARAM(wparam) == 1)
				{
					OnMouseDown(pt.x, Size.y - 1 - pt.y, key::XButton1);
					OnKeyDown(key::XButton1);
				}
				if (GET_XBUTTON_WPARAM(wparam) == 2)
				{
					OnMouseDown(pt.x, Size.y - 1 - pt.y, key::XButton2);
					OnKeyDown(key::XButton2);
				}
			} break;
			case WM_SYSKEYDOWN: OnKeyDown(static_cast<key>(wparam)); return 0; break;
			case WM_KEYDOWN: OnKeyDown(static_cast<key>(wparam)); break;
			case WM_LBUTTONUP: {
				const POINTS pt = MAKEPOINTS(lparam);
				OnMouseUp(pt.x, Size.y - 1 - pt.y, key::LButton);
				OnKeyUp(key::LButton);
			}  break;
			case WM_MBUTTONUP: {
				const POINTS pt = MAKEPOINTS(lparam);
				OnMouseUp(pt.x, Size.y - 1 - pt.y, key::MButton);
				OnKeyUp(key::MButton);
			} break; 
			case WM_RBUTTONUP: {
				const POINTS pt = MAKEPOINTS(lparam);
				OnMouseUp(pt.x, Size.y - 1 - pt.y, key::RButton);
				OnKeyUp(key::RButton);
			} break; 
			case WM_XBUTTONUP: {
				const POINTS pt = MAKEPOINTS(lparam);
				if (GET_XBUTTON_WPARAM(wparam) == 1)
				{
					OnMouseUp(pt.x, Size.y - 1 - pt.y, key::XButton1);
					OnKeyUp(key::XButton1);
				}
				if (GET_XBUTTON_WPARAM(wparam) == 2)
				{
					OnMouseUp(pt.x, Size.y - 1 - pt.y, key::XButton2);
					OnKeyUp(key::XButton2);
				}
			} break; 
			case WM_SYSKEYUP: OnKeyUp(static_cast<key>(wparam)); return 0; break;
			case WM_KEYUP: OnKeyUp(static_cast<key>(wparam)); break;
				   
			case WM_MOUSEWHEEL: OnMouseWheel((float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WheelDelta); break;
			case WM_MOUSEHWHEEL: OnMouseWheel((float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WheelDelta); break;
			case WM_MOUSEMOVE: {
				const POINTS pt = MAKEPOINTS(lparam);
				OnMouseMove(pt.x, Size.y - 1 - pt.y);
			} break;
			case WM_MOUSELEAVE: OnMouseLeave(); break;

			case WM_CHAR: OnChar(static_cast<char32_t>(wparam)); if (wparam == '\r') OnChar('\n'); break;
			case WM_SYSCHAR: OnChar(static_cast<char32_t>(wparam)); return 0; break;
			case WM_UNICHAR: OnUniChar(static_cast<char32_t>(wparam)); break;

			default: break;
			}
		}
		catch (...)
		{
			CurrentWindowException = std::current_exception();
		}
		return DefWindowProcW(static_cast<HWND>(hwnd), static_cast<UINT>(msg), wparam, lparam);
	}

	inline bool Window::OnClose()
	{
		return Closing ? Closing() : false;
	}
	inline void Window::OnResize(int32_t w, int32_t h)
	{
		if (Resize) Resize(w, h);
	}
	inline void Window::OnKillFocus()
	{
		Input.CrearState();
		if (KillFocus) KillFocus();
	}
	inline void Window::OnMouseDown(int32_t x, int32_t y, key k)
	{
		if (MouseDown) MouseDown(x, y, k);
	}
	inline void Window::OnMouseMove(int32_t x, int32_t y)
	{
		Input.OnMouseMove(x - Size.x / 2.0f, y - Size.y / 2.0f);
		if (MouseMove) MouseMove(x, y);
	}
	inline void Window::OnMouseUp(int32_t x, int32_t y, key k)
	{
		if (MouseUp) MouseUp(x, y, k);
	}
	inline void Window::OnMouseWheel(float d)
	{
		Input.OnMouseWheel(d);
		if (MouseWheel) MouseWheel(d);
	}
	inline void Window::OnMouseHWheel(float d)
	{
		if (MouseHWheel) MouseHWheel(d);
	}
	inline void Window::OnMouseLeave()
	{
		if (MouseLeave) MouseLeave();
	}
	inline void Window::OnKeyDown(key k)
	{
		Input.OnKeyDown(k);
		if (KeyDown) KeyDown(k);
	}
	inline void Window::OnKeyUp(key k)
	{
		Input.OnKeyUp(k);
		if (KeyUp) KeyUp(k);
	}
	inline void Window::OnChar(char32_t c)
	{
		Input.OnChar(c);
		if (Char) Char(c);
	}
	inline void Window::OnUniChar(char32_t c)
	{
		if (UniChar) UniChar(c);
	}

	Window::Window(const std::string& title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx) :
		Size{ 0 }, CursorHandle{ 0 }, WindowHandle{ 0 }, WindowPlace{ new WINDOWPLACEMENT }
	{
		WindowStyle = static_cast<DWORD>(s);
		WindowStyleEx = static_cast<DWORD>(sx);

		LPCWSTR cl = WindowsInitializer::GetName();
		HINSTANCE inst = static_cast<HINSTANCE>(WindowsInitializer::GetInstance());
		RECT wr = { 0, 0, w, h };
		AdjustWindowRectEx(&wr, WindowStyle, false, WindowStyleEx);
		w = wr.right - wr.left;
		h = wr.bottom - wr.top;

		if (!(WindowHandle = CreateWindowExW(WindowStyleEx, cl, (LPWSTR)to_utf16(title).c_str(), 
			WindowStyle, x, y, w, h, nullptr, nullptr, inst, this)))
			throw std::runtime_error("airs::Window error: Window creation failed.");
	}
	Window::~Window()
	{
		if (WindowHandle) DestroyWindow(static_cast<HWND>(WindowHandle));
		delete WindowPlace;
	}
	void Window::SetTitle(const std::string& title)
	{
		SetWindowTextW(static_cast<HWND>(WindowHandle), (LPWSTR)to_utf16(title).c_str());
	}
	void Window::SetCursor(const Cursor& c)
	{
		if (CursorHandle == c.Handle) return;
		CursorHandle = c.Handle;
		PostMessage(static_cast<HWND>(WindowHandle), WM_SETCURSOR, 0, 1);
	}
	void Window::SetFullscreen(bool f)
	{
		if (f == Fullscreen) return;

		if (f) 
		{
			GetWindowPlacement(static_cast<HWND>(WindowHandle), (PWINDOWPLACEMENT)WindowPlace);

			LONG NewStyle = WindowStyle;
			NewStyle &= ~WS_BORDER;
			NewStyle &= ~WS_DLGFRAME;
			NewStyle &= ~WS_THICKFRAME;

			LONG NewStyleEx = WindowStyleEx;
			NewStyleEx &= ~WS_EX_WINDOWEDGE;

			SetWindowLongW(static_cast<HWND>(WindowHandle), GWL_STYLE, NewStyle | WS_POPUP);
			SetWindowLongW(static_cast<HWND>(WindowHandle), GWL_EXSTYLE, NewStyleEx | WS_EX_TOPMOST);
			ShowWindow(static_cast<HWND>(WindowHandle), SW_SHOWMAXIMIZED);
		}
		else 
		{
			SetWindowLongW(static_cast<HWND>(WindowHandle), GWL_STYLE, WindowStyle);
			SetWindowLongW(static_cast<HWND>(WindowHandle), GWL_EXSTYLE, WindowStyleEx);
			ShowWindow(static_cast<HWND>(WindowHandle), SW_SHOWNORMAL);
			SetWindowPlacement(static_cast<HWND>(WindowHandle), (PWINDOWPLACEMENT)WindowPlace);
		}

		Fullscreen = f;
	}
	int32_t Window::Width() const
	{
		return Size.x;
	}
	int32_t Window::Height() const
	{
		return Size.y;
	}
	vec2i Window::ClientSize() const
	{
		return Size;
	}
	void Window::Show(Appear ws)
	{
		ShowWindow(static_cast<HWND>(WindowHandle), static_cast<int>(ws));
	}
	void Window::Close()
	{
		DestroyWindow(static_cast<HWND>(WindowHandle)); 
		WindowHandle = nullptr;
	}
	void Window::ProcessMessages()
	{
		MSG msg;
		while (PeekMessageW(&msg, static_cast<HWND>(WindowHandle), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			if (CurrentWindowException)
			{
				auto t = CurrentWindowException;
				CurrentWindowException = nullptr;
				std::rethrow_exception(t);
			}
		}
	}
	void Window::PrecessThreadMessages()
	{
		MSG msg;
		while (PeekMessageW(&msg, (HWND)(-1), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			if (CurrentWindowException)
			{
				auto t = CurrentWindowException;
				CurrentWindowException = nullptr;
				std::rethrow_exception(t);
			}
		}
	}
}

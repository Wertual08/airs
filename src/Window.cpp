#include "airs/Window.hpp"
#include "airs/Utilities.hpp"
#include <stdexcept>
#include <Windows.h>
#include <Windowsx.h>



namespace airs
{
	class WindowsInitializer
	{
	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			return static_cast<LRESULT>(reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA))->
				OnMessage(hwnd, static_cast<uint32_t>(msg), static_cast<uint64_t>(wparam), static_cast<int64_t>(lparam)));
		}
		static LRESULT CALLBACK Startup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			if (msg == WM_NCCREATE)
			{
				Window* p = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCT>(lparam)->lpCreateParams);
				if (p != nullptr)
				{
					SetWindowLongPtrW(static_cast<HWND>(hwnd), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc));
					SetWindowLongPtrW(static_cast<HWND>(hwnd), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p));
					return static_cast<LRESULT>(p->OnMessage(hwnd, static_cast<uint32_t>(msg), static_cast<uint64_t>(wparam), static_cast<int64_t>(lparam)));
				}
			}
			return DefWindowProcW(static_cast<HWND>(hwnd), msg, wparam, lparam);
		}

		static constexpr LPCWSTR Name = L"AIRS_Framework_Window";
		ATOM Atom;

	public:
		constexpr LPCWSTR GetName() { return Name; }
		ATOM GetAtom() { return Atom; }

		WindowsInitializer()
		{
			WNDCLASSEXW wc = { 0 };
			wc.cbSize = sizeof(wc);
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = &Startup;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = GetModuleHandleW(nullptr);
			wc.hIcon = nullptr;
			wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
			wc.hbrBackground = CreateSolidBrush(0);
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = Name;
			wc.hIconSm = nullptr;
			if ((Atom = RegisterClassExW(&wc)) == 0) throw std::runtime_error("airs::WindowsInitializer error: Can not register window class.");
		}
		~WindowsInitializer()
		{
			UnregisterClassW(Name, GetModuleHandleW(nullptr));
		}
	};


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
	vec2f Window::UserInput::MouseDelta()
	{
		vec2f delta(DeltaX, DeltaY);
		DeltaX = 0.0f;
		DeltaY = 0.0f;
		return delta;
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
			case WM_PAINT: OnPaint(); break;
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

			case WM_MOUSEWHEEL: {
				POINT pt;
				pt.x = GET_X_LPARAM(lparam);
				pt.x = GET_Y_LPARAM(lparam);
				ScreenToClient(static_cast<HWND>(WindowHandle), &pt);
				OnMouseWheel(pt.x, Size.y - 1 - pt.y, (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WheelDelta); 
			} break;
			case WM_MOUSEHWHEEL: {
				POINT pt;
				pt.x = GET_X_LPARAM(lparam);
				pt.x = GET_Y_LPARAM(lparam);
				ScreenToClient(static_cast<HWND>(WindowHandle), &pt);
				OnMouseWheel(pt.x, Size.y - 1 - pt.y, (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WheelDelta);
			} break;
			case WM_MOUSEMOVE: OnMouseMove(GET_X_LPARAM(lparam), Size.y - 1 - GET_Y_LPARAM(lparam)); break;
			case WM_MOUSELEAVE: OnMouseLeave(); break;

			case WM_CHAR: OnChar(static_cast<char32_t>(wparam)); if (wparam == '\r') OnChar('\n'); break;
			case WM_SYSCHAR: OnChar(static_cast<char32_t>(wparam)); return 0; break;
			case WM_UNICHAR: OnUniChar(static_cast<char32_t>(wparam)); break;

			case WM_ENTERSIZEMOVE: OnEnterInternalLoop(); break;
			case WM_EXITSIZEMOVE: OnExitInternalLoop(); break;

			default: break;
			}
		}
		catch (...)
		{
			CurrentWindowException = std::current_exception();
		}
		return DefWindowProcW(static_cast<HWND>(hwnd), static_cast<UINT>(msg), static_cast<WPARAM>(wparam), static_cast<LPARAM>(lparam));
	}

	inline bool Window::OnClose()
	{
		return Closing ? Closing() : false;
	}
	inline void Window::OnResize(int32_t w, int32_t h)
	{
		if (Resize) Resize(w, h);
	}
	inline void Window::OnPaint()
	{
		if (Paint) Paint();
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
	inline void Window::OnMouseUp(int32_t x, int32_t y, key k)
	{
		if (MouseUp) MouseUp(x, y, k);
	}
	inline void Window::OnMouseWheel(std::int32_t x, std::int32_t y, float d)
	{
		Input.OnMouseWheel(d);
		if (MouseWheel) MouseWheel(x, y, d);
	}
	inline void Window::OnMouseHWheel(std::int32_t x, std::int32_t y, float d)
	{
		if (MouseHWheel) MouseHWheel(x, y, d);
	}
	inline void Window::OnMouseMove(std::int32_t x, std::int32_t y)
	{
		Input.OnMouseMove(x - Size.x / 2.0f, y - Size.y / 2.0f);
		if (MouseMove) MouseMove(x, y);
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

	inline void Window::OnEnterInternalLoop()
	{
		if (EnterInternalLoop) EnterInternalLoop();
	}
	inline void Window::OnExitInternalLoop()
	{
		if (ExitInternalLoop) ExitInternalLoop();
	}

	Window::Window() : Fullscreen(false), CurrentWindowException(nullptr), WindowStyle(0), 
		WindowStyleEx(0), Size(0), CursorHandle(nullptr), WindowHandle(nullptr)
	{
	}
	void Window::Init(const std::string& title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx)
	{
		static WindowsInitializer WindowsInstance;

		Fullscreen = false;
		WindowStyle = static_cast<DWORD>(s);
		WindowStyleEx = static_cast<DWORD>(sx);

		RECT wr = { 0, 0, w, h };
		AdjustWindowRectEx(&wr, WindowStyle, false, WindowStyleEx);
		w = wr.right - wr.left;
		h = wr.bottom - wr.top;

		WindowHandle = CreateWindowExW(WindowStyleEx, WindowsInstance.GetName(), to_wide(title).c_str(),
			WindowStyle, x, y, w, h, nullptr, nullptr, GetModuleHandleW(nullptr), this);
		if (!WindowHandle) throw std::runtime_error("airs::Window error: Window creation failed.");
	}

	Window::Window(const std::string& title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx) :
		Fullscreen(false), CurrentWindowException(nullptr), WindowStyle(static_cast<DWORD>(s)), WindowStyleEx(static_cast<DWORD>(sx)),
		Size(0), CursorHandle(nullptr), WindowHandle(nullptr)
	{
		Init(title, w, h, x, y, s, sx);
	}
	Window::~Window()
	{
		if (WindowHandle) DestroyWindow(static_cast<HWND>(WindowHandle));
		WindowHandle = nullptr;
	}
	void Window::SetTitle(const std::string& title)
	{
		SetWindowTextW(static_cast<HWND>(WindowHandle), to_wide(title).c_str());
	}
	void Window::SetCursor(const Cursor& c)
	{
		if (CursorHandle == c.Handle) return;
		CursorHandle = c.Handle;
		PostMessageW(static_cast<HWND>(WindowHandle), WM_SETCURSOR, 0, 1);
	}
	void Window::SetFullscreen(bool f)
	{
		if (f == Fullscreen) return;

		if (f)
		{
			GetWindowPlacement(static_cast<HWND>(WindowHandle), reinterpret_cast<PWINDOWPLACEMENT>(&WindowPlace));

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
			SetWindowPlacement(static_cast<HWND>(WindowHandle), reinterpret_cast<PWINDOWPLACEMENT>(&WindowPlace));
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

	void Window::CheckException()
	{
		if (CurrentWindowException)
		{
			auto t = CurrentWindowException;
			CurrentWindowException = nullptr;
			std::rethrow_exception(t);
		}
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
	void Window::ProcessThreadMessages()
	{
		MSG msg;
		while (PeekMessageW(&msg, (HWND)(-1), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Window::Draw(int32_t x, int32_t y, const Surface& s)
	{
		uint32_t* buffer;

		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = s.Width();
		bmi.bmiHeader.biHeight = s.Height();
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biClrUsed = 1;

		HDC hWndDc = GetDC(static_cast<HWND>(WindowHandle));
		if (!hWndDc) throw std::runtime_error("airs::Window::Draw error: Can not create device context");
		HBITMAP hDib = CreateDIBSection(hWndDc, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&buffer), 0, 0);
		if (buffer == NULL) throw std::runtime_error("airs::Window::Draw error: Can not create DIBSection.");

		size_t size = s.Size();
		const uint32_t* src = static_cast<const uint32_t*>(s);
		while (size--)
		{
			*buffer = (*src << 24) | (*src >> 8);
			src++;
			buffer++;
		}

		HDC hDibDC = CreateCompatibleDC(hWndDc);
		HGDIOBJ hOldObj = SelectObject(hDibDC, hDib);

		BitBlt(hWndDc, x, Size.y - 1 - y - s.Height(), s.Width(), s.Height(), hDibDC, 0, 0, SRCCOPY);

		SelectObject(hDibDC, hOldObj);
		DeleteDC(hDibDC);

		DeleteObject(hDib);

		ReleaseDC(static_cast<HWND>(WindowHandle), hWndDc);
	}
	void Window::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
	{
		HDC hWndDc = GetDC(static_cast<HWND>(WindowHandle));
		RECT rect;
		rect.left = x;
		rect.top = Size.y - 1 - y - h;
		rect.right = x + w;
		rect.bottom = Size.y - 1 - y;
		FillRect(hWndDc, &rect, CreateSolidBrush((color << 24) | (color >> 8)));
		ReleaseDC(static_cast<HWND>(WindowHandle), hWndDc);
	}

	void* Window::Handle() const
	{
		return WindowHandle;
	}
}

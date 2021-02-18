#pragma once
#include "delegate.hpp"
#include "Cursor.hpp"
#include "KeyCodes.hpp"
#include "math.hpp"
#include "Surface.hpp"
#include <cstdint>
#include <string>
#include <bitset>
#include <array>



namespace airs
{
	class Window
	{
		friend class WindowsInitializer;
		friend class MessageBox;

	public:
		static constexpr int32_t WheelDelta = 120;
		static constexpr int32_t UseDefault = (int32_t)0x80000000;
		enum class Style : uint32_t
		{
			Overlapped = 0x00000000L,
			Popup = 0x80000000L,
			Child = 0x40000000L,
			Minimize = 0x20000000L,
			Visible = 0x10000000L,
			Disabled = 0x08000000L,
			ClipSiblings = 0x04000000L,
			ClipChildren = 0x02000000L,
			Maximize = 0x01000000L,
			Caption = 0x00C00000L,
			Border = 0x00800000L,
			DlgFrame = 0x00400000L,
			VScroll = 0x00200000L,
			HScroll = 0x00100000L,
			SysMenu = 0x00080000L,
			ThickFrame = 0x00040000L,
			Group = 0x00020000L,
			TabStop = 0x00010000L,
			MinimizeBox = 0x00020000L,
			MaximizeBox = 0x00010000L,
			Tiled = 0x00000000L,
			Iconic = 0x20000000L,
			SizeBox = 0x00040000L,
			OverlappedWindow = 0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000 | 0x00010000,
			TiledWindow = OverlappedWindow,
			PopupWindow = 0x80000000L | 0x00800000L | 0x00080000L,
			ChildWindow = 0x40000000L,

			ByteAlingClient = 0x1000,
			ByteAlignWindow = 0x2000,
			ClassDC = 0x0040,
			DoubleClicks = 0x0008,
			DropShadow = 0x00020000,
			GlobalClass = 0x4000,
			HRedraw = 0x0002,
			NoClose = 0x0200,
			OwnDC = 0x0020,
			ParentDC = 0x0080,
			SaceBits = 0x0800,
			VRedraw = 0x0001,

			Default = Overlapped | Caption | SysMenu | MinimizeBox | MaximizeBox | ThickFrame | OwnDC,
		};
		enum class StyleEx : uint32_t
		{
			AcceptFiles = 0x00000010L,
			AppWindow = 0x00040000L,
			ClientEdge = 0x00000200L,
			Composited = 0x02000000L,
			ContextHelp = 0x00000400L,
			ControlParrent = 0x00010000L,
			DlgModalFrame = 0x00000001L,
			Layered = 0x00080000,
			LayoutRTL = 0x00400000L,
			Left = 0x00000000L,
			LeftScrollBar = 0x00004000L,
			LTRReading = 0x00000000L,
			MDIChild = 0x00000040L,
			NoAtivate = 0x08000000L,
			NoInheritLayout = 0x00100000L,
			NoParentNotify = 0x00000004L,
			TopMost = 0x00000008L,
			Transparent = 0x00000020L,
			ToolWindow = 0x00000080L,
			WindowEdge = 0x00000100L,

			Default = 0,
		};
		enum class Appear : int32_t
		{
			Hide = 0,
			Normal = 1,
			Minimized = 2,
			Maximized = 3,
			Maximize = 3,
			NoActive = 4,
			Show = 5,
			Minimize = 6,
			MinNoActive = 7,
			NA = 8,
			Restore = 9,
			Default = 10,
			ForceMinimize = 11,
		};
		
	private:
		class UserInput
		{
			friend class Window;
		private:
			bool CharsLocked;
			float CursorX, CursorY;
			float DeltaX, DeltaY;
			float Wheel;
			unsigned char RingReader, RingWriter;
			std::array<char32_t, 256> CharsRing;
			std::bitset<256> KeyStates;

			UserInput();
			UserInput(const UserInput&) = delete;
			~UserInput();
			UserInput& operator=(const UserInput&) = delete;

			void SetCursor(float x, float y);
			void OnMouseMove(float x, float y);
			void OnMouseWheel(float d);
			void OnKeyDown(Key b);
			void OnKeyUp(Key b);
			bool OnChar(char32_t c);
			void CrearState();

		public:
			void LockChars(bool flushkeystates = true);
			void UnlockChars();
			bool KeyState(Key b) const;
			bool operator[](Key b) const;
			float MouseX() const;
			float MouseY() const;
			vec2f Mouse() const;
			float WheelDelta();
			float MouseDeltaX();
			float MouseDeltaY();
			vec2f MouseDelta();
			char32_t GetChar();
			void Clear();
		};

		std::exception_ptr CurrentWindowException;

		struct
		{
			uint32_t Length = 0;
			uint32_t Flags = 0;
			uint32_t ShowCmd = 0;
			vec2i MinPosition;
			vec2i MaxPosition;
			vec4i NormalPosition;
			vec4i Device;
		} WindowPlace;
		bool Fullscreen = false;

		int64_t OnMessage(void* hwnd, uint32_t msg, uint64_t wparam, int64_t lparam);
		
	protected:
		void* WindowHandle;
		void* CursorHandle;
		uint32_t WindowStyle;
		uint32_t WindowStyleEx;
		vec2i Size;
		
		inline virtual bool OnClose();
		inline virtual void OnResize(std::int32_t w, std::int32_t h);
		inline virtual void OnPaint();
		inline virtual void OnKillFocus();
		inline virtual void OnMouseDown(std::int32_t x, std::int32_t y, Key k);
		inline virtual void OnMouseUp(std::int32_t x, int32_t y, Key k);
		inline virtual void OnMouseWheel(std::int32_t x, std::int32_t y, float d);
		inline virtual void OnMouseHWheel(std::int32_t x, std::int32_t y, float d);
		inline virtual void OnMouseMove(std::int32_t x, std::int32_t y);
		inline virtual void OnMouseLeave();
		inline virtual void OnKeyDown(Key k);
		inline virtual void OnKeyUp(Key k);
		inline virtual void OnChar(char32_t c);
		inline virtual void OnUniChar(char32_t c);

		inline virtual void OnEnterInternalLoop();
		inline virtual void OnExitInternalLoop();

		Window();
		void Init(const std::string& title, int32_t w, int32_t h, int32_t x = UseDefault, int32_t y = UseDefault, Style s = Style::Default, StyleEx sx = StyleEx::Default);

	public:
		UserInput Input;

		Window(const std::string& title, int32_t w, int32_t h, int32_t x = UseDefault, int32_t y = UseDefault, Style s = Style::Default, StyleEx sx = StyleEx::Default);
		virtual ~Window();
		void SetTitle(const std::string& title);
		void SetCursor(const Cursor &c);
		void SetFullscreen(bool f);
		int32_t Width() const;
		int32_t Height() const;
		vec2i ClientSize() const;

		void Show(Appear ws = Appear::Default);
		void Close();

		void CheckException();
		void ProcessMessages();
		static void ProcessThreadMessages();

		void Draw(int32_t x, int32_t y, const Surface& s);
		void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

		void* Handle() const;

		delegate<bool()> Closing;
		delegate<void(int32_t, int32_t)> Resize;
		delegate<void()> Paint;
		delegate<void()> KillFocus;

		delegate<void(std::int32_t, std::int32_t, Key)> MouseDown;
		delegate<void(std::int32_t, std::int32_t, Key)> MouseUp;
		delegate<void(std::int32_t, std::int32_t, float)> MouseWheel;
		delegate<void(std::int32_t, std::int32_t, float)> MouseHWheel;
		delegate<void(std::int32_t, std::int32_t)> MouseMove;
		delegate<void()> MouseLeave;
		delegate<void(Key)> KeyDown;
		delegate<void(Key)> KeyUp;
		delegate<void(char32_t)> Char;
		delegate<void(char32_t)> UniChar;

		delegate<void()> EnterInternalLoop;
		delegate<void()> ExitInternalLoop;
	};
	static constexpr inline Window::Style operator|(Window::Style a, Window::Style b)
	{
		return static_cast<Window::Style>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}
	static constexpr inline Window::StyleEx operator|(Window::StyleEx a, Window::StyleEx b)
	{
		return static_cast<Window::StyleEx>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}
}


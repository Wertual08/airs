#pragma once
#include <airs/math.hpp>
#include <airs/KeyCodes.hpp>



namespace airs::gi
{
	class EventArgs
	{
	private:
		bool IsHandled = false;

	public:
		bool Handled() const { return IsHandled; }
		void Handle() { IsHandled = true; }
	};

	class MouseEventArgs : public EventArgs
	{
	private:
		vec2i MousePosition;
		vec2i MouseDelta;
		vec2f MouseWheelDelta;
		Key KeyCode;

	public:
		MouseEventArgs(airs::vec2i pos, airs::vec2i delta, vec2f wheel, Key key) :
			MousePosition(pos),
			MouseDelta(delta),
			MouseWheelDelta(wheel),
			KeyCode(key)
		{
		}

		vec2i Position() const { return MousePosition; }
		vec2i Delta() const { return MouseDelta; }
		vec2f WheelDelta() const { return MouseWheelDelta; }
		Key Key() const { return KeyCode; }
	};

	class KeyboardEventArgs : public EventArgs
	{
	private:
		Key KeyCode;
		char32_t CharCode;

	public:
		KeyboardEventArgs(Key key, char32_t code) :
			KeyCode(key),
			CharCode(code)
		{
		}

		Key Key() const { return KeyCode; }
		char32_t Char() const { return CharCode; }
	};
}
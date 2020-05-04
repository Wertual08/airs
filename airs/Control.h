#pragma once
#include "Graphics.h"
#include "KeyCodes.h"
#include "delegate.h"
#include "math.h"



namespace airs
{
	class Control
	{
	private:
		size_t LastLayers;
		bool Dirty, Visible;

	protected:
		vec2i Position;
		vec2i Size; 

	public:
		struct Event
		{
		private:
			bool IsHandled = false, IsVisited = false;
		public:
			void Handle() { IsHandled = true; }
			bool Handled() const { return IsHandled; }
			void Visit() { IsVisited = true; }
			bool Visited() const { return IsVisited; }
		};
		struct MouseEvent : Event
		{
		private:
			vec2i PosGlobal, PosLocal;
			key MouseKey;
		public:
			MouseEvent(vec2i gl, vec2i loc, key k = key::None) : PosGlobal(gl), PosLocal(loc), MouseKey(k) {}
			vec2i Global() const { return PosGlobal; }
			vec2i Local() const { return PosLocal; }
			key Key() const { return MouseKey; }
		};
		struct KeyboardEvent : Event
		{
		private:
			key KeyboardKey;
			char32_t Symbol;
		public:
			KeyboardEvent(key k, char32_t s) : KeyboardKey(k), Symbol(s) {}
			key Key() const { return KeyboardKey; }
			char32_t Char() const { return Symbol; }
		};

		Control();
		Control(const Control& c);
		Control(vec2i pos, vec2i size);

		bool UnderPoint(float x, float y) const;
		bool UnderPoint(vec2i vec) const;

		virtual void SetPosition(vec2i vec);
		vec2i GetPosition() const;
		void AdjustPosition(vec2i vec);
		virtual void SetSize(vec2i vec);
		vec2i GetSize() const;
		void AdjustSize(vec2i vec);
		virtual void SetVisible(bool v);
		bool GetVisible() const;
		virtual void Refresh();

		virtual uint32_t Layers() const;

		virtual void OnMouseDown(MouseEvent& e);
		virtual void OnMouseMove(MouseEvent& e);
		virtual void OnMouseUp(MouseEvent& e);

		virtual void OnChar(KeyboardEvent& e);
		virtual void OnKeyDown(KeyboardEvent& e);
		virtual void OnKeyUp(KeyboardEvent& e);

		virtual void Render(Graphics& gfx) const;

		void RequestRender(Graphics& gfx);

		delegate<void(vec2i)> Resize;
		delegate<void(vec2i)> Move;
	};
}


#pragma once
#include <cstdint>
#include "KeyCodes.hpp"



namespace airs
{
	class LayeredWindow;
	class Layer
	{
		friend class LayeredWindow;

	private:
		LayeredWindow* ActiveWindow;

	protected:
		bool Activate(LayeredWindow* window);

		virtual bool OnActivate();
		virtual bool OnClose();
		virtual bool OnFocus();
		virtual bool OnKillFocus();
		virtual bool OnMouseDown(int32_t x, int32_t y, Key k);
		virtual bool OnMouseMove(int32_t x, int32_t y);
		virtual bool OnMouseUp(int32_t x, int32_t y, Key k);
		virtual bool OnMouseWheel(float d);
		virtual bool OnMouseHWheel(float d);
		virtual bool OnKeyDown(Key k);
		virtual bool OnKeyUp(Key k);
		virtual bool OnChar(char32_t c);
		virtual bool OnUniChar(char32_t c);

	public:
		Layer();

		bool Close();
		bool Closed() const;
		bool Opened() const;
	};
}
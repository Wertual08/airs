#pragma once
#include <vector>
#include "math.h"
#include "Control.h"             
#include "VertexArray.h"
#include "Spriteset.h"
#include "Charset.h"



namespace airs
{
	class Layout
	{
	private:
		VertexArray Renderer;
		mutable Buffer RenderData;
		mutable uint32_t DataCapacity;

	protected:
		vec2i Position;
		bool ClosedStatus;

		std::vector<Control*> Controls;

	public:
		Layout(const Layout&) = delete;
		Layout& operator=(const Layout&) = delete;

		Layout(Layout&& f) noexcept;
		Layout();
		Layout(vec2i pos);
		
		virtual bool OnMouseDown(vec2i pos, key k);
		virtual bool OnMouseMove(vec2i pos);
		virtual bool OnMouseUp(vec2i pos, key k);
					 
		virtual bool OnChar(char32_t c);
		virtual bool OnKeyDown(key k);
		virtual bool OnKeyUp(key k);

		virtual void Render(const std::vector<uint32_t>& imgid, Charset& chars) const;

		void Add(Control& c);
		Control& operator[](size_t i);
		const Control& operator[](size_t i) const;

		void SetPosition(vec2i p);
		vec2i GetPostion() const;
		void AdjustPosition(vec2i o);

		void Open();
		void Close();
		bool Closed() const;

		delegate<void(vec2i)> Move;
	};
}


#include "airs/Layout.h"
#include "airs/Graphics.h"         



namespace airs
{
	Layout::Layout(Layout&& f) noexcept : RenderData(std::move(f.RenderData)), Renderer(std::move(f.Renderer))
	{
		std::swap(Position, f.Position);
		std::swap(ClosedStatus, f.ClosedStatus);
		std::swap(DataCapacity, f.DataCapacity);
	}
	Layout::Layout() : Layout(0.0f)
	{
	}
	Layout::Layout(vec2i pos) : Position(pos), RenderData(airs::Buffer::Target::Array), 
		DataCapacity(0), ClosedStatus(true)
	{
		Renderer.AttribIPointer(RenderData, 0, 4, VertexArray::Int, sizeof(Graphics::Vertex), (void*)0);
		Renderer.AttribIPointer(RenderData, 1, 3, VertexArray::UInt, sizeof(Graphics::Vertex), (void*)16);
	}

	bool Layout::OnMouseDown(vec2i pos, key k)
	{
		Control::MouseEvent Event(pos, pos - Position, k);
		for (size_t i = Controls.size(); i > 0; i--)
			Controls[i - 1]->OnMouseDown(Event);
		
		return Event.Handled();
	}
	bool Layout::OnMouseMove(vec2i pos)
	{
		Control::MouseEvent Event(pos, pos - Position);
		for (size_t i = Controls.size(); i > 0; i--)
			Controls[i - 1]->OnMouseMove(Event);
		return Event.Handled();
	}
	bool Layout::OnMouseUp(vec2i pos, key k)
	{
		Control::MouseEvent Event(pos, pos - Position, k);
		for (size_t i = Controls.size(); i > 0; i--)
			Controls[i - 1]->OnMouseUp(Event);
		return Event.Handled();
	}

	bool Layout::OnChar(char32_t c)
	{
		Control::KeyboardEvent Event(key::None, c);
		for (size_t i = Controls.size(); i > 0; i--)
			Controls[i - 1]->OnChar(Event);
		return Event.Handled();
	}
	bool Layout::OnKeyDown(key k)
	{
		Control::KeyboardEvent Event(k, 0);
		for (size_t i = Controls.size(); i > 0; i--)
			Controls[i - 1]->OnKeyDown(Event);
		return Event.Handled();
	}
	bool Layout::OnKeyUp(key k)
	{
		Control::KeyboardEvent Event(k, 0);
		for (size_t i = Controls.size(); i > 0; i--)
			Controls[i - 1]->OnKeyUp(Event);
		return Event.Handled();
	}
	
	void Layout::Render(const std::vector<uint32_t>& imgid, Charset& chars) const
	{
		Graphics Gfx(imgid, chars, RenderData, DataCapacity);
		for (size_t i = 0; i < Controls.size(); i++)
			Controls[i]->RequestRender(Gfx);
		Renderer.Render(VertexArray::Points, Gfx.GetCursor());
	}

	void Layout::Add(Control& c)
	{
		Controls.push_back(&c);
	}
	Control& Layout::operator[](size_t i)
	{
		return *Controls[i];
	}
	const Control& Layout::operator[](size_t i) const
	{
		return *Controls[i];
	}
	
	void Layout::SetPosition(vec2i p)
	{
		Position = p;
	}
	vec2i Layout::GetPostion() const
	{
		return Position;
	}
	void Layout::AdjustPosition(vec2i o)
	{
		SetPosition(Position + o);
	}

	void Layout::Open()
	{
		ClosedStatus = false;
	}
	void Layout::Close()
	{
		ClosedStatus = true;
	}
	bool Layout::Closed() const
	{
		return ClosedStatus;
	}
}
#pragma once
#include "Element.hpp"
#include <vector>



namespace airs::gi
{
	class Container : public Element
	{
	private:
		std::vector<Element*> Elements;

	public:
		void SetPosition(airs::vec2i pos) override
		{
			Element::SetPosition(pos);
			for (Element* element : Elements) element->ApplyConstraints(GetPosition(), GetExtent());
		}
		void SetExtent(airs::vec2i ext) override
		{
			Element::SetExtent(ext);
			for (Element* element : Elements) element->ApplyConstraints(GetPosition(), GetExtent());
		}

		virtual bool IsContainer() const final override { return true; }

		bool OnMouseDown(vec2i pos, key key) override
		{
			std::size_t i = Elements.size();
			while (i--) if (Elements[i]->OnMouseDown(pos, key)) return true;
			return false;
		}
		bool OnMouseUp(vec2i pos, key key) override
		{
			std::size_t i = Elements.size();
			while (i--) if (Elements[i]->OnMouseUp(pos, key)) return true;
			return false;
		}
		bool OnMouseWheel(vec2i pos, float delta) override
		{
			std::size_t i = Elements.size();
			while (i--) if (Elements[i]->OnMouseWheel(pos, delta)) return true;
			return false;
		}
		bool OnMouseHWheel(vec2i pos, float delta) override
		{
			std::size_t i = Elements.size();
			while (i--) if (Elements[i]->OnMouseHWheel(pos, delta)) return true;
			return false;
		}
		bool OnMouseMove(vec2i pos) override
		{
			bool handled = false;
			std::size_t i = Elements.size();
			while (i--) if (Elements[i]->OnMouseMove(pos)) handled = true;
			return handled;
		}
		bool OnKeyDown(key key) override
		{
			std::size_t i = Elements.size();
			while (i--) if (Elements[i]->OnKeyDown(key)) return true;
			return false;
		}
		bool OnKeyUp(key key) override
		{
			std::size_t i = Elements.size();
			while (i--) if (Elements[i]->OnKeyUp(key)) return true;
			return false;
		}
		bool OnChar(char32_t c) override
		{
			std::size_t i = Elements.size();
			while (i--) if (Elements[i]->OnChar(c)) return true;
			return false;
		}

		void Add(Element& element)
		{
			element.SetOwner(*this);
			element.ApplyConstraints(GetPosition(), GetExtent());
			Elements.push_back(&element);
		}
		void Remove(Element* element)
		{
			std::size_t j = 0;
			for (size_t i = 0; i < Elements.size(); i++)
				if (Elements[i] != element) Elements[j++] = Elements[i];
			Elements.resize(j);
		}
		std::size_t Size() const { return Elements.size(); }
		Element* operator[](std::size_t i) const { return Elements[i]; }

		virtual void Render(Renderer& renderer) { for (Element* element : Elements) element->Render(renderer); }
	};
}
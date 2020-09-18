#pragma once
#include "Element.hpp"



namespace airs::gi
{
	class Button : public Element
	{
    private:
        Renderer::Image ReleasedImage;
        Renderer::Image SelectedImage;
        Renderer::Image PressedImage;
        bool Selected = false;
        bool Pressed = false;

    public:
        Renderer::Image GetReleasedImage() const { return ReleasedImage; }
        void SetReleasedImage(Renderer::Image img) { ReleasedImage = img; }
        Renderer::Image GetSelectedImage() const { return SelectedImage; }
        void SetSelectedImage(Renderer::Image img) { SelectedImage = img; }
        Renderer::Image GetPressedImage() const { return PressedImage; }
        void SetPressedImage(Renderer::Image img) { PressedImage = img; }

        virtual bool OnMouseDown(vec2i pos, key key) override
        {
            if (Contains(pos) && key == key::LButton)
            {
                Pressed = true;
                return true;
            }
            else return false;
        }
        virtual bool OnMouseUp(vec2i pos, key key) override
        {
            bool result = Pressed;
            Pressed = false;
            return result;
        }
        virtual bool OnMouseMove(vec2i pos) override
        {
            if (Pressed) return true;
            else if (Contains(pos))
            {
                Selected = true;
                return true;
            }
            else
            {
                Selected = false;
                return false;
            }
        }

        void Render(Renderer& renderer) override 
        { 
            if (Pressed) renderer.Push(GetPosition(), GetExtent(), PressedImage);
            else if (Selected) renderer.Push(GetPosition(), GetExtent(), SelectedImage);
            else renderer.Push(GetPosition(), GetExtent(), ReleasedImage);
        }
	};
}
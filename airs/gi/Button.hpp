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

    protected:
        virtual void OnMouseDown(MouseEventArgs& args) override
        {
            if (!args.Handled() && Contains(args.Position()) && args.Key() == Key::LButton)
            {
                Pressed = true;
                args.Handle();
            }
        }
        virtual void OnMouseUp(MouseEventArgs& args) override
        {
            if (Pressed) args.Handle();
            Pressed = false;
        }
        virtual void OnMouseMove(MouseEventArgs& args) override
        {
            if (Pressed) args.Handle();
            else if (!args.Handled() && Contains(args.Position()))
            {
                Selected = true; 
                args.Handled();
            }
            else Selected = false;
        }

    public:
        Renderer::Image GetReleasedImage() const { return ReleasedImage; }
        void SetReleasedImage(Renderer::Image img) { ReleasedImage = img; }
        Renderer::Image GetSelectedImage() const { return SelectedImage; }
        void SetSelectedImage(Renderer::Image img) { SelectedImage = img; }
        Renderer::Image GetPressedImage() const { return PressedImage; }
        void SetPressedImage(Renderer::Image img) { PressedImage = img; }

        void Render(Renderer& renderer) override 
        { 
            if (Pressed) renderer.Push(GetPosition(), GetExtent(), PressedImage);
            else if (Selected) renderer.Push(GetPosition(), GetExtent(), SelectedImage);
            else renderer.Push(GetPosition(), GetExtent(), ReleasedImage);
        }
	};
}
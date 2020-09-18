#pragma once
#include "Element.hpp"



namespace airs::gi
{
    class Panel : public Element
    {
    protected:
        Renderer::Image Image;

    public:
        Renderer::Image GetImage() const { return Image; }
        void SetImage(Renderer::Image img) { Image = img; }

        void Render(Renderer& renderer) override { renderer.Push(GetPosition(), GetExtent(), Image); }
    };
}
#pragma once
#include "Element.hpp"



namespace airs::gi
{
    class Label : public Element
    {
    protected:
        std::uint32_t Color;
        std::string Text;

    public:
        std::uint32_t GetColor() const { return Color; }
        void SetColor(std::uint32_t color) { Color = color; }
        
        const std::string &GetText() const { return Text; }
        void SetText(const std::string text) { Text = text; }


        void Render(Renderer &renderer) override { renderer.Push(GetPosition(), Color, Text); }
    };
}
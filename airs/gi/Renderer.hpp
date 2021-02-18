#pragma once
#include <cstdint>
#include <string>
#include <airs/math.hpp>



namespace airs::gi
{
    class Renderer
    {
    public:
        struct Image
        {
            enum StretchType : std::int32_t
            {
                Solid,
                Repeat,
                Scale,
                Glyph,
            };
            std::uint32_t Color = 0x888888ffu;
            std::int32_t Texture = -1;
            StretchType Stretch = Solid;

            bool operator==(const Image &img) const
            {
                return Color == img.Color && Texture == img.Texture && Stretch == img.Stretch;
            }
            bool operator!=(const Image &img) const
            {
                return Color != img.Color || Texture != img.Texture || Stretch != img.Stretch;
            }
        };

        virtual void Push(airs::vec2i pos, airs::vec2i ext, Image img) = 0;
        virtual void Push(airs::vec2i pos, std::uint32_t color, const std::string &str) = 0;
    };
}
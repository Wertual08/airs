#include "airs/Graphics.hpp"



namespace airs
{
    inline void AlphaBlend(uint32_t& o, uint32_t n)
    {
        uint64_t alpha = n & 0xffull;
        uint32_t rb = o & 0xff00ff00u;
        uint32_t g = o & 0x00ff0000u;
        rb += static_cast<uint32_t>(((n & 0xff00ff00ull) - rb) * alpha >> 8);
        g += static_cast<uint32_t>(((n & 0x00ff0000ull) - g) * alpha >> 8);
        o = (rb & 0xff00ff00u) | (g & 0x00ff0000u) | std::max(static_cast<uint32_t>(alpha), o & 0xffu);
    }
    inline uint32_t ColorMult(uint32_t c, uint32_t m)
    {
        uint32_t rb = c & 0xff00ff00u;
        uint32_t ga = c & 0x00ff00ffu;
        return (((static_cast<uint64_t>(rb) * m) >> 8) & 0xff00ff00u) | (((static_cast<uint64_t>(ga) * m) >> 8) & 0x00ff00ffu);
    }

    Graphics::Graphics(Surface& surface) : ActiveSurface(surface)
    {
    }

    void Graphics::Fill(uint32_t color)
    {
        size_t size = ActiveSurface.Size();
        while (size--) (static_cast<uint32_t*>(ActiveSurface))[size] = color;
    }
    void Graphics::Draw(int32_t x, int32_t y, const Surface& s)
    {
        int32_t hy = s.Height();
        size_t src_row = s.Width();
        size_t dst_row = ActiveSurface.Width();
        const uint32_t* src = s;
        uint32_t* dst = ActiveSurface;
        dst += static_cast<size_t>(y) * dst_row + x;
        while (hy--)
        {
            memcpy(dst, src, src_row);
            src += src_row;
            dst += dst_row;
        }
    }
    void Graphics::Draw(int32_t x, int32_t y, const Glyph& s, uint32_t color)
    {
        union converter
        {
            uint32_t hex;
            vec4ub rgba;
        };

        int32_t hy = s.BitmapH();
        size_t src_row = s.BitmapW();
        size_t dst_row = ActiveSurface.Width();
        const uint8_t* src = s;
        uint32_t* dst = ActiveSurface;
        dst += static_cast<size_t>(y) * dst_row + x;
        while (hy--)
        {
            for (size_t lx = 0; lx < src_row; lx++)
            {
                converter instance = { color };
                instance.rgba = vec4us(instance.rgba) * src[lx] / 255;
                AlphaBlend(dst[lx], ColorMult(color, src[lx]));
            }
            src += src_row;
            dst += dst_row;
        }
    }
    void Graphics::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
    {
        size_t dst_row = ActiveSurface.Width();
        uint32_t* dst = static_cast<uint32_t*>(ActiveSurface) + static_cast<size_t>(y) * dst_row + x;
        while (h--)
        {
            for (int32_t lx = 0; lx < w; lx++) AlphaBlend(dst[lx], color);
            dst += dst_row;
        }
    }
}
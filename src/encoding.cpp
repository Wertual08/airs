#include "airs/encoding.hpp"
#include <codecvt>
#include <istream>
#include <vector>



namespace airs
{
    std::string to_utf8(const std::wstring &s)
    {
        return std::move(std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t>().to_bytes(s));
    }
    std::string to_utf8(const std::u16string &s)
    {
        return std::move(std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t>().to_bytes(s));
    }
    std::string to_utf8(const std::u32string &s)
    {
        return std::move(std::wstring_convert<std::codecvt<char32_t, char, std::mbstate_t>, char32_t>().to_bytes(s));
    }

    std::u16string to_utf16(const std::string &s)
    {
        return std::move(std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t>().from_bytes(s));
    }
    std::u16string to_utf16(const std::wstring &s)
    {
        return std::move(to_utf16(to_utf8(s)));
    }
    std::u16string to_utf16(const std::u32string &s)
    {
        return std::move(to_utf16(to_utf8(s)));
    }

    std::u32string to_utf32(const std::string &s)
    {
        return std::move(std::wstring_convert<std::codecvt<char32_t, char, std::mbstate_t>, char32_t>().from_bytes(s));
    }
    std::u32string to_utf32(const std::wstring &s)
    {
        return std::move(to_utf32(to_utf8(s)));
    }
    std::u32string to_utf32(const std::u16string &s)
    {
        return std::move(to_utf32(to_utf8(s)));
    }

    std::wstring to_wide(const std::string &s)
    {
        return std::move(std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t>().from_bytes(s));
    }
    std::wstring to_wide(const std::u16string &s)
    {
        return std::move(to_wide(to_utf8(s)));
    }
    std::wstring to_wide(const std::u32string &s)
    {
        return std::move(to_wide(to_utf8(s)));
    }

    std::u32string read_with_bom(std::istream &src)
    {
        enum class encoding {
            encoding_utf32be = 0,
            encoding_utf32le,
            encoding_utf16be,
            encoding_utf16le,
            encoding_utf8,
            encoding_ascii,
        };

        std::vector<std::string> boms = {
            std::string("\x00\x00\xFE\xFF", 4),
            std::string("\xFF\xFE\x00\x00", 4),
            std::string("\xFE\xFF", 2),
            std::string("\xFF\xFE", 2),
            std::string("\xEF\xBB\xBF", 3)
        };

        std::string buffer((std::istreambuf_iterator<char>(src)), std::istreambuf_iterator<char>());

        encoding enc = encoding::encoding_ascii;

        for (size_t i = 0; i < boms.size(); ++i) {
            std::string testBom = boms[i];
            if (buffer.compare(0, testBom.length(), testBom) == 0) {
                enc = encoding(i);
                buffer = buffer.substr(testBom.length());
                break;
            }
        }

        switch (enc) {
        case encoding::encoding_utf32be:
        {
            if (buffer.length() % 4 != 0) {
                throw std::logic_error("size in bytes must be a multiple of 4");
            }
            size_t count = buffer.length() / 4;
            std::u32string temp = std::u32string(count, 0);
            for (size_t i = 0; i < count; ++i) {
                temp[i] = static_cast<char32_t>(buffer[i * 4ull + 3] << 0 | buffer[i * 4 + 2] << 8 | buffer[i * 4 + 1] << 16 | buffer[i * 4 + 0] << 24);
            }
            return temp;
        }
        case encoding::encoding_utf32le:
        {
            if (buffer.length() % 4 != 0) {
                throw std::logic_error("size in bytes must be a multiple of 4");
            }
            size_t count = buffer.length() / 4;
            std::u32string temp = std::u32string(count, 0);
            for (size_t i = 0; i < count; ++i) {
                temp[i] = static_cast<char32_t>(buffer[i * 4 + 0] << 0 | buffer[i * 4 + 1] << 8 | buffer[i * 4 + 2] << 16 | buffer[i * 4 + 3] << 24);
            }
            return temp;
        }
        case encoding::encoding_utf16be:
        {
            if (buffer.length() % 2 != 0) {
                throw std::logic_error("size in bytes must be a multiple of 2");
            }
            size_t count = buffer.length() / 2;
            std::u16string temp = std::u16string(count, 0);
            for (size_t i = 0; i < count; ++i) {
                temp[i] = static_cast<char16_t>(buffer[i * 2 + 1] << 0 | buffer[i * 2 + 0] << 8);
            }
            return to_utf32(temp);
        }
        case encoding::encoding_utf16le:
        {
            if (buffer.length() % 2 != 0) {
                throw std::logic_error("size in bytes must be a multiple of 2");
            }
            size_t count = buffer.length() / 2;
            std::u16string temp = std::u16string(count, 0);
            for (size_t i = 0; i < count; ++i) {
                temp[i] = static_cast<char16_t>(buffer[i * 2 + 0] << 0 | buffer[i * 2 + 1] << 8);
            }
            return to_utf32(temp);
        }
        default:
            return to_utf32(buffer);
        }
    }

    char32_t utf8_read(const std::string &s, std::size_t &cur)
    {
        constexpr char32_t error = U'\xffff';
        static constexpr char utf8_lengths[] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0
        };
        static constexpr int utf8_shiftc[] = {
            0, 18, 12, 6, 0
        };
        static constexpr char utf8_masks[] = {
            0, 0b01111111, 0b00011111,
            0b00001111, 0b00000111
        };

        if (cur >= s.size()) return U'\0';

        unsigned char f = static_cast<unsigned char>(s[cur]);
        std::size_t len = utf8_lengths[f >> 3];
        if (len < 1)
        {
            cur++;
            return error;
        }

        char32_t res = static_cast<char32_t>(f & utf8_masks[len]) << 18;
        if (cur + len > s.size())
        {
            cur++;
            return error;
        }

        switch (len) 
        {
        case 4: res |= static_cast<char32_t>(s[cur + 3] & 0x3f) << 0;
        case 3: res |= static_cast<char32_t>(s[cur + 2] & 0x3f) << 6;
        case 2: res |= static_cast<char32_t>(s[cur + 1] & 0x3f) << 12;
        }
        res >>= utf8_shiftc[len];
        cur += len;
        return res;
    }
}

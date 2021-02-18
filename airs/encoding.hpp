#pragma once
#include <string>



namespace airs
{
	std::string to_utf8(const std::wstring &s);
	std::string to_utf8(const std::u16string &s);
	std::string to_utf8(const std::u32string &s);

	std::u16string to_utf16(const std::string &s);
	std::u16string to_utf16(const std::wstring &s);
	std::u16string to_utf16(const std::u32string &s);

	std::u32string to_utf32(const std::string &s);
	std::u32string to_utf32(const std::wstring &s);
	std::u32string to_utf32(const std::u16string &s);

	std::wstring to_wide(const std::string &s);
	std::wstring to_wide(const std::u16string &s);
	std::wstring to_wide(const std::u32string &s);

	std::u32string read_with_bom(std::istream &src);

	char32_t utf8_read(const std::string &s, std::size_t &cur);
}
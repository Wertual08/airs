#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <istream>
#include <ostream>



namespace airs
{
    std::vector<std::uint8_t> read_bytes(const std::string &filename);

    template<typename T> struct bin_t { T data; };
    template<typename T> bin_t<T &> bin(T &data) { return bin_t<T &>{data}; }
    template<typename T> bin_t<const T &> bin(const T &data) { return bin_t<const T &>{data}; }

    template<typename T>
    std::istream &operator>>(std::istream &is, bin_t<T> b) {
        return is.read(reinterpret_cast<char *>(&b.data), sizeof(T));
    }
    template<typename T>
    std::ostream &&operator<<(std::ostream &os, bin_t<T &> b) {
        return os.write(reinterpret_cast<char *>(&b.data), sizeof(T));
    }


    template<typename T> struct bin_array_t { T *data; std::size_t count; };
    template<typename T> bin_array_t<T> bin(T *data, std::size_t count) { return bin_array_t<T>{data, count}; }
    template<typename T> bin_array_t<const T> bin(const T *data, std::size_t count) { return bin_array_t<const T>{data, count}; }
    template<typename T> bin_array_t<T> bin(std::vector<T> &vec) { return bin_array_t<T>{vec.data(), vec.size()}; }
    template<typename T> bin_array_t<const T> bin(std::vector<const T> &vec) { return bin_array_t<const T>{ vec.data(), vec.size() }; }
    bin_array_t<char> bin(std::string &str) { return { str.data(), str.size() }; }
    bin_array_t<char16_t> bin(std::u16string &str) { return { str.data(), str.size() }; }
    bin_array_t<char32_t> bin(std::u32string &str) { return { str.data(), str.size() }; }

    template<typename T>
    std::istream &operator>>(std::istream &is, bin_array_t<T> b) {
        return is.read(reinterpret_cast<char *>(b.data), b.count * sizeof(T));
    }
    template<typename T>
    std::ostream &&operator<<(std::ostream &os, bin_array_t<T &> b) {
        return os.write(reinterpret_cast<char *>(b.data), b.count * sizeof(T));
    }
}

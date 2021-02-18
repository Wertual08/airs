#include "airs/binary.hpp"
#include <fstream>



namespace airs
{
    std::vector<std::uint8_t> read_bytes(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) throw std::runtime_error("ReadFile error: Failed to open file [" + filename + "].");
        std::size_t fileSize = static_cast<std::size_t>(file.tellg());
        std::vector<std::uint8_t> buffer(fileSize);
        file.seekg(0);
        file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
        return std::move(buffer);
    }
}
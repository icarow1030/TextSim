#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

class SHA256 {
public:
    SHA256();
    static std::string hash(const std::string& input);
    static std::string hash_file(const std::string& filename);

private:
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr size_t HASH_SIZE = 32;
    static constexpr size_t ORIGINAL_SIZE = 8;
    static const uint32_t K[64];

    static void process_block(const uint8_t* data, uint32_t* h);
    static std::vector<uint8_t> pad(const uint8_t* data, size_t length);
    static std::vector<uint8_t> read_file(const std::string& filename);
    static std::string to_hex_string(const uint32_t* hash);
};

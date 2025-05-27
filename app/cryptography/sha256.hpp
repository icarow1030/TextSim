#pragma once
#include <string>
#include <vector>
#include <cstdint>

class SHA256 {
public:
    SHA256();
    std::string hash(const std::string& input);
    std::string hash_file(const std::string& filename);

private:
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr size_t HASH_SIZE = 32;
    static constexpr size_t ORIGINAL_SIZE = 8;
    static const uint32_t K[64];

    void process_block(const uint8_t* data, uint32_t* h);
    std::vector<uint8_t> pad(const uint8_t* data, size_t length);
    std::vector<uint8_t> read_file(const std::string& filename);
    std::string to_hex_string(const uint32_t* hash);
};

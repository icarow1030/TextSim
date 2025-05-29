#include "sha256.hpp"

const uint32_t SHA256::K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

namespace {
inline uint32_t ROTRIGHT(uint32_t word, uint32_t bits) {
    return (word >> bits) | (word << (32 - bits));
}
inline uint32_t CH(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}
inline uint32_t MAJ(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}
inline uint32_t EP0(uint32_t x) {
    return ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22);
}
inline uint32_t EP1(uint32_t x) {
    return ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25);
}
inline uint32_t SIG0(uint32_t x) {
    return ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ (x >> 3);
}
inline uint32_t SIG1(uint32_t x) {
    return ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ (x >> 10);
}
}

SHA256::SHA256() {}

std::string SHA256::hash(const std::string& input) {
    std::vector<uint8_t> data(input.begin(), input.end());
    std::vector<uint8_t> padded = pad(data.data(), data.size());
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    size_t blocks = padded.size() / BLOCK_SIZE;
    for (size_t i = 0; i < blocks; ++i) {
        process_block(&padded[i * BLOCK_SIZE], h);
    }
    return to_hex_string(h);
}

std::string SHA256::hash_file(const std::string& filename) {
    std::vector<uint8_t> file_data = read_file(filename);
    if (file_data.empty()) return "";
    std::vector<uint8_t> padded = pad(file_data.data(), file_data.size());
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    size_t blocks = padded.size() / BLOCK_SIZE;
    for (size_t i = 0; i < blocks; ++i) {
        process_block(&padded[i * BLOCK_SIZE], h);
    }
    return to_hex_string(h);
}

void SHA256::process_block(const uint8_t* data, uint32_t* h) {
    uint32_t w[64] = {0};
    for (int j = 0; j < 16; ++j) {
        w[j] = (data[j * 4] << 24) | (data[j * 4 + 1] << 16) |
               (data[j * 4 + 2] << 8) | (data[j * 4 + 3]);
    }
    for (int j = 16; j < 64; ++j) {
        w[j] = SIG1(w[j - 2]) + w[j - 7] + SIG0(w[j - 15]) + w[j - 16];
    }
    uint32_t temp_h[8];
    for (int j = 0; j < 8; ++j) temp_h[j] = h[j];
    for (int j = 0; j < 64; ++j) {
        uint32_t T1 = temp_h[7] + EP1(temp_h[4]) + CH(temp_h[4], temp_h[5], temp_h[6]) + K[j] + w[j];
        uint32_t T2 = EP0(temp_h[0]) + MAJ(temp_h[0], temp_h[1], temp_h[2]);
        temp_h[7] = temp_h[6];
        temp_h[6] = temp_h[5];
        temp_h[5] = temp_h[4];
        temp_h[4] = temp_h[3] + T1;
        temp_h[3] = temp_h[2];
        temp_h[2] = temp_h[1];
        temp_h[1] = temp_h[0];
        temp_h[0] = T1 + T2;
    }
    for (int j = 0; j < 8; ++j) {
        h[j] += temp_h[j];
    }
}

std::vector<uint8_t> SHA256::pad(const uint8_t* data, size_t length) {
    size_t total_length = length + 1 + ORIGINAL_SIZE;
    size_t next_multiple_of_512 = ((total_length + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;
    std::vector<uint8_t> padded(next_multiple_of_512, 0);
    memcpy(padded.data(), data, length);
    padded[length] = 0x80;
    uint64_t original_size = static_cast<uint64_t>(length) * 8;
    for (int i = 0; i < 8; ++i) {
        padded[next_multiple_of_512 - 8 + i] = (original_size >> (56 - i * 8)) & 0xFF;
    }
    return padded;
}

std::vector<uint8_t> SHA256::read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return {};
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return buffer;
}

std::string SHA256::to_hex_string(const uint32_t* hash) {
    std::ostringstream oss;
    for (int i = 0; i < 8; ++i) {
        oss << std::hex << std::setw(8) << std::setfill('0') << hash[i];
    }
    return oss.str();
}
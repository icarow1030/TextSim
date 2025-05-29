#pragma once

#include <string>
#include <vector>
#include <stdexcept>


class Base64 {
    public:
        static std::string encode(const std::vector<unsigned char>& input);
        static std::vector<unsigned char> decode(const std::string& input);
    private:
        static const std::string base64_chars;
        static bool is_base64(unsigned char c);

};
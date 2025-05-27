#pragma once
#include <string>

class Keys {
private:
    std::string n, e, d;
    bool initialized = false;

public:
    Keys();

    void initialize();  // Gera e armazena chaves internamente (chama rsa::generateKeys)
    std::string encrypt(const std::string& msg) const;
    std::string decrypt(const std::string& cipher) const;
};

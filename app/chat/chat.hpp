#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include "../config/config.hpp"
#include <thread>
#include "style.hpp"
#include "../cryptography/rsa.hpp"
#include "../cryptography/sha256.hpp"
#include "../cryptography/base64.hpp"
#include "chat.hpp"
#include <iostream>
#include "../config/config.hpp"
#include "../client/AppClient.hpp"
#include <thread>
#include <httplib.h>
#include <atomic>

// Forward declaration for AppClient
class AppClient;

#include "../server/AppServer.hpp"

using namespace nlohmann;

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

class Chat {
    public:
        void addMessage(const json& message, bool is_local = false);
        void printMessages() const;
        void chat(AppClient& client);
        void clearTerminal() const;
        json getMessages() const;
        void changeUsernames(const std::string& new_username);
    private:
        std::vector<json> messages;
        bool isMe(const json& message) const {
            return message.value("user_id", "") == Config::User::USER_ID;
        }
        void printHeader() const;
        std::string hashMessage(const json& message) const;
        std::string encryptAndEncode(const std::string& value, const RSA::PublicKey& key) const;
        void encryptMessageFields(json& message, const RSA::PublicKey& key) const;
};
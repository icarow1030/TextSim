#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include "../config/config.hpp"
#include <thread>
#include "style.hpp"
#include "../cryptography/rsa.hpp"
#include "../cryptography/sha256.hpp"

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
            return message.value("username", Config::Default::USERNAME) == Config::User::USERNAME;
        }
};
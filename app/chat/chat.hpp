#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include "../config/config.hpp"
#include <thread>

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
        void addMessage(const json& message);
        void printMessages() const;
        void chat(AppClient& client);
        void clearTerminal() const;
    private:
        std::vector<json> messages;
        bool isMe(const json& message) const {
            return message.value("username", Config::Default::USERNAME) == Config::User::USERNAME;
        }
};
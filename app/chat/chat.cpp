#include "chat.hpp"
#include <iostream>
#include "../config/config.hpp"
#include "../client/AppClient.hpp"

void Chat::clearTerminal() const {
    std::system(CLEAR_COMMAND);
}

void Chat::addMessage(const json& message) {
    messages.push_back(message);
}

void Chat::printMessages() const {
    clearTerminal();
    std::cout << "[ --- CHAT --- ]" << std::endl;
    for(const auto& msg : messages) {
        std::string username = msg.value("username", Config::Default::USERNAME);
        if(isMe(msg)) {
            username = "you";
        }
        std::string content = msg.value("content", Config::Default::MESSAGE);
        std::cout << username << ": " << content << std::endl;
    }
}

void Chat::chat(AppClient& client) {
    printMessages();
    std::string input;
    std::cout << "Type your message (or '/quit' to quit): ";
    while(true) {
        std::getline(std::cin, input);
        if(input == "/quit") {
            std::cout << "Exiting chat..." << std::endl;
            break;
        }
        if(input.empty()) {
            std::cout << "Message cannot be empty. Try again: ";
            continue;
        } else {
            json message;
            message["username"] = Config::User::USERNAME;
            message["content"] = input;
            client.sendMessage(message);
            printMessages();
            std::cout << "Type your message (or '/quit' to quit): ";
        }
    }
}
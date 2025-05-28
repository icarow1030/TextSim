#include "chat.hpp"
#include <iostream>
#include "../config/config.hpp"
#include "../client/AppClient.hpp"
#include <thread>
#include <httplib.h>
#include <atomic>

void Chat::clearTerminal() const {
    std::system(CLEAR_COMMAND);
}

void Chat::addMessage(const json& message, bool is_local) {
    // Evita duplicação se a mensagem já foi adicionada
    static std::string last_message;
    std::string current = message.dump();
    
    if(current != last_message) {
        messages.push_back(message);
        last_message = current;
        printMessages();
        if(!is_local) {
            std::cout << "Type your message (or '/quit' to quit): ";
            std::cout.flush();
        }
    }
}

json Chat::getMessages() const {
    return messages;
}

void Chat::changeUsernames(const std::string& new_username) {
    for(auto& msg : messages) {
        if(msg.contains("username")) {
            msg["username"] = new_username;
        }
    }
}

void Chat::printMessages() const {
    clearTerminal();
        // Cabeçalho ASCII simples
    std::cout << TerminalStyle::BOLD 
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD 
              << "|" << TerminalStyle::BG_CYAN << "          *  C H A T  *          " 
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD 
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n\n";
    for(const auto& msg : messages) {
        std::string username = msg.value("username", Config::Default::USERNAME);
        std::string content = msg.value("content", Config::Default::MESSAGE);

        std::string user_color = (username == Config::User::USERNAME)
        ? TerminalStyle::GREEN : TerminalStyle::CYAN;

        std::cout << TerminalStyle::BOLD << user_color
                  << std::left << std::setw(15) << (username + ":")
                  << TerminalStyle::RESET << " " << content << "\n";
    }
    std::cout << TerminalStyle::YELLOW 
              << "+--------------------------------------+" 
              << TerminalStyle::RESET << "\n";
}


void Chat::chat(AppClient& client) {
    printMessages();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer
    std::string input;
    
    while(true) {
        std::cout << "Type your message (or '/quit' to quit): ";
        std::getline(std::cin, input);
        
        if(input == "/quit") break;
        if(input.empty()) continue;
        
        json message;
        message["user_id"] = Config::User::USER_ID;
        message["username"] = Config::User::USERNAME;
        message["content"] = input;
        message["timestamp"] = std::chrono::system_clock::now().time_since_epoch().count();
        message["pub_key"] = {
            {"n", Config::User::PUBLIC_KEY.n.get_str()},
            {"e", Config::User::PUBLIC_KEY.e.get_str()}
        };
        std::string string_to_hash = message["user_id"].get<std::string>() + 
            message["username"].get<std::string>() + 
            message["content"].get<std::string>();
        message["hash"] = SHA256::hash(string_to_hash);
        // Adiciona localmente marcando como própria mensagem
        addMessage(message, true);
        
        message["user_id"] = RSA::encrypt(message["user_id"].get<std::string>(), Config::User::PUBLIC_KEY);

        // Envia para o outro cliente
        if(client.sendMessage(message)) {
            std::cout << "Message sent!" << std::endl;
        }
    }
}
#include <string>
#include <iostream>
#include <memory>
#include "AppClient.hpp"
#include "../server/AppServer.hpp"

AppClient::AppClient() {}

//bool AppClient::handshake() {
    // Perform a handshake with the target server
    //This includes authentication and RSA key exchange
    //const int target_port = Config::Server::TARGER_PORT;

//};

void AppClient::sendMessage(json message) {
    if(client == nullptr) {
        std::cerr << "Client not initialized. Call generateClient() first." << std::endl;
        return;
    }
    std::string body = message.dump();
    httplib::Headers headers = { {"Content-Type", "application/json"} };
    auto res = client->Post("/message", headers, body, "application/json");
    if (res && res->status == 200) {
        std::cout << "Message sent successfully!" << std::endl;
    } else if (res) {
        std::cerr << "Failed to send message. Status: " << res->status << std::endl;
    } else {
        std::cerr << "Failed to send message. No response received." << std::endl;
    }
}

void AppClient::generateClient() {
    client = std::make_unique<httplib::Client>(Config::Server::HOST, server_ptr->getPort());
}


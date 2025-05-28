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

httplib::Result AppClient::sendMessage(json message) {
    httplib::Client client(Config::Server::TARGET_HOST, Config::Server::TARGER_PORT);
    std::string body = message.dump();
    if (body.empty()) {
        std::cerr << "Message body is empty. Cannot send." << std::endl;
        return httplib::Result(nullptr, httplib::Error::Unknown);
    }
    httplib::Headers headers = { {"Content-Type", "application/json"} };
    auto res = client.Post("/message", headers, body, "application/json");
    if (res && res->status == 200) {
    } else if(res) {
        std::cerr << "Failed to send message. Status: " << res->status << std::endl;
    } else {
        std::cerr << "Failed to send message. No response received." << std::endl;
    }
    return res;
}


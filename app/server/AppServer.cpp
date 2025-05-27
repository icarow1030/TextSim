#include "../config/config.hpp"
#include "AppServer.hpp"
#include <httplib.h>
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include <nlohmann/json.hpp> // For JSON parsing
#include "../chat/chat.hpp"

using namespace httplib;

AppServer::AppServer(Chat& chatRef)
    : server_status(Config::ServerStatus::OFFLINE),
      port(Config::Server::PORT),
      host(Config::Server::HOST),
      chat(chatRef) // inicializa a referência
{}

int AppServer::getPort() const {
    return port;
}

void AppServer::updatePort(int new_port) {
    if (new_port <= 0 || new_port > 65535) {
        throw std::invalid_argument("Port number must be between 1 and 65535.");
    }
    port = new_port;
    
}

void AppServer::setupRoutes() {

    server.Get("/", [](const Request& req, Response& res) {
        res.set_content("Welcome to the server!", "text/plain");
    });

    server.Get("/checkConnection", [](const Request& req, Response& res) {
        res.set_content("Server is running", "text/plain");
    });

    server.Post("/message", [this](const Request& req, Response& res) {
        handleMessage(req);
        res.set_content("Message received", "text/plain");
    });
}

void AppServer::start() {
    try {
        setupRoutes();

        server_thread = std::thread([this]() {
            try {
                server.listen(host, port);
            } catch(const std::exception& e) {
                std::cerr << "Error in server thread: " << e.what() << std::endl;
                server_status = Config::ServerStatus::OFFLINE;
            }
        });
        server_status = Config::ServerStatus::ONLINE;
        server_thread.detach();
    } catch(const std::exception& e) {
        std::cerr << "Error starting server: " << e.what() << std::endl;
        server_status = Config::ServerStatus::ERROR_;
        return;
    }
}

void AppServer::stop() {
    server.stop();
    server_status = Config::ServerStatus::OFFLINE;
}

Config::ServerStatus AppServer::status() {
    return server_status;
}

void AppServer::handleMessage(const Request& req) {
    // Handle incoming messages
    try {
        auto json_body = nlohmann::json::parse(req.body);
        std::string message = json_body.value("content", "");
        if (message.empty()) {
            throw std::invalid_argument("Message cannot be empty");
        }
        // Verificação e autenticação
        // Caso passe na autenticação, armazena a mensagem chamando o "addMessage" do Chat
        chat.addMessage(json_body);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing message: " << e.what() << std::endl;
        throw;
    }
}
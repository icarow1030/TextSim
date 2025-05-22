#include "../config/config.hpp"
#include "AppServer.hpp"
#include <httplib.h>
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>

using namespace httplib;

AppServer::AppServer() : server_status(Config::ServerStatus::OFFLINE), port(Config::Server::PORT), host(Config::Server::HOST) {

}

int AppServer::getPort() const {
    return port;
}

void AppServer::updatePort() {
    port = Config::Server::PORT;
}

void AppServer::setupRoutes() {

    server.Get("/", [](const Request& req, Response& res) {
        res.set_content("Welcome to the server!", "text/plain");
    });

    server.Get("/checkConnection", [](const Request& req, Response& res) {
        res.set_content("Server is running", "text/plain");
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
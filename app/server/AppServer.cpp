#include "../config/config.hpp"
#include "AppServer.hpp"
#include <httplib.h>
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace httplib;

AppServer::AppServer() : connection_status(Config::Connection::DISCONNECTED), port(Config::Server::PORT), host(Config::Server::HOST) {

}

int AppServer::getPort() const {
    return port;
}

void AppServer::updatePort() {
    port = Config::Server::PORT;
}

void AppServer::start() {
    try {
        server.listen(host, port);
    } catch(const std::exception& e) {
        std::cerr << "Error starting server: " << e.what() << std::endl;
        connection_status = Config::Connection::ERROR_AT_CONNECTION;
        return;
    }
}

void AppServer::stop() {
    server.stop();
    connection_status = Config::Connection::DISCONNECTED;
}

Config::Connection AppServer::status() {
    return connection_status;
}
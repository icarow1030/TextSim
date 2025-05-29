#pragma once
#include <httplib.h>
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include <nlohmann/json.hpp> // For JSON parsing
#include "../chat/chat.hpp"
#include "../config/config.hpp"

// Forward declaration for Chat
class Chat;

class AppServer {

    public:
        AppServer(Chat& chatRef);

        void handshakeHandle();
        void start();
        void stop();
        void setupRoutes();
        int getPort() const;
        void updatePort(int new_port);
        Config::ServerStatus status();
        inline httplib::Server& getServer() {
            return server;
        };

    private:
        httplib::Server server;
        Config::ServerStatus server_status;
        int port;
        std::string host;
        std::thread server_thread;
        Chat& chat; // Add Chat reference as a member

        void handleMessage(const httplib::Request& req);
};
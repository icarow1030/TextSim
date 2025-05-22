#pragma once
#include <httplib.h>
#include "../config/config.hpp"
#include <string>
#include <iostream>
#include <thread>

class AppServer {

    public:
        AppServer();

        void start();
        void stop();
        void setupRoutes();
        int getPort() const;
        void updatePort();
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
};
#pragma once
#include <httplib.h>
#include "../config/config.hpp"
#include <string>
#include <iostream>

class AppServer {

    public:
        AppServer();

        void start();
        void stop();
        int getPort() const;
        void updatePort();
        Config::Connection status();

    private:
        httplib::Server server;
        Config::Connection connection_status;
        int port;
        std::string host;
};
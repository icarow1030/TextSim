#pragma once
#include <string>
#include <iostream>
#include <stdexcept>
#include "../cryptography/rsa.hpp"

namespace Config {

    inline constexpr const char* APP_NAME = "TextSim";
    inline constexpr const char* APP_VERSION = "0.0.1";
    inline constexpr const char* APP_AUTHOR = "YcroSamsara";

    enum class Connection {
        CONNECTED,
        DISCONNECTED,
        CONNECTING,
        DISCONNECTING,
        ERROR_AT_CONNECTION
    };

    enum class ServerStatus {
        ONLINE,
        OFFLINE,
        ERROR_
    };

    enum class ClientStatus {
        ONLINE,
        OFFLINE,
        ERROR_
    };

    namespace Default {
        inline constexpr const char* USERNAME = "Anonymous";
        inline constexpr const char* MESSAGE = "Hello, World!";
        inline constexpr int PORT = 8071;
        inline constexpr int TARGET_PORT = 8070;
    }

    namespace User {
        extern std::string USERNAME;
        inline std::string USER_ID;
        inline RSA::PublicKey PUBLIC_KEY;
        inline RSA::PrivateKey PRIVATE_KEY;
    }

    namespace Server {
        extern int PORT;
        inline constexpr const char* HOST = "localhost";
        extern int TARGER_PORT;
        inline constexpr const char* TARGET_HOST = "localhost";
    }

    namespace Routes {
        inline constexpr const char* SEND = "/send";
    }

    namespace Limits {
        inline constexpr int MAX_USERNAME_LENGTH = 20;
        inline constexpr int MAX_MESSAGE_LENGTH = 1024;
    }

    inline void set_username(const std::string& username) {
        if (username.length() > Limits::MAX_USERNAME_LENGTH) {
            throw std::length_error("Username exceeds maximum length");
        }
        User::USERNAME = username;
    }

    inline void set_port(int port) {
        if (port == Server::TARGER_PORT) {
            throw std::invalid_argument("Port cannot be the same as the target port");
        }
        if (port <= 0 || port > 65535) {
            throw std::out_of_range("Port must be between 1 and 65535");
        }
        Server::PORT = port;
    }

    inline void set_target_port(int port) {
        if (port == Server::PORT) {
            throw std::invalid_argument("Target port cannot be the same as the current port");
        }
        if (port <= 0 || port > 65535) {
            throw std::out_of_range("Target port must be between 1 and 65535");
        }
        Server::TARGER_PORT = port;
    }

    inline std::string get_base_url() {
        return std::string("http://") + Server::HOST + ":" + std::to_string(Server::PORT);
    }

    inline std::string get_target_url() {
        return std::string("http://") + Server::TARGET_HOST + ":" + std::to_string(Server::TARGER_PORT);
    }
}
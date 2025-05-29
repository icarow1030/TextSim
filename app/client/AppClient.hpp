#pragma once
#include <httplib.h>
#include "../config/config.hpp"
#include <memory>
#include <nlohmann/json.hpp> // For JSON parsing
#include "../cryptography/rsa.hpp"
#include "../cryptography/sha256.hpp"
#include <string>
#include <iostream>
#include <memory>
#include "../server/AppServer.hpp"

using namespace nlohmann;

// Forward declaration for AppServer
class AppServer;

class AppClient {
    public:
        AppClient();
        void handshakeSend(const std::string target_host, const int target_port); // Perform a handshake with the target server - Authentication and RSA Keys
        httplib::Result sendMessage(json message); // Send a message to the server
        Config::ClientStatus status(); // Get the client status
        void setServerPtr(AppServer* ptr) { server_ptr = ptr; } // Set the server pointer

    private:
        AppServer* server_ptr; // Use raw pointer to avoid circular dependency
        std::string username; // username (used for authentication)
        std::string hostname; // host + port (used for authentication)
        Config::ClientStatus client_status; // Generate the client with the server information
};

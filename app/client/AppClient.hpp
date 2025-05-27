#pragma once
#include <httplib.h>
#include "../config/config.hpp"
#include <memory>
#include <nlohmann/json.hpp> // For JSON parsing

using namespace nlohmann;

// Forward declaration for AppServer
class AppServer;

class AppClient {
    public:
        AppClient();
        bool handshake(); // Perform a handshake with the target server - Authentication and RSA Keys
        void sendMessage(json message); // Send a message to the server
        Config::ClientStatus status(); // Get the client status
        void generateClient();
        void setServerPtr(AppServer* ptr) { server_ptr = ptr; } // Set the server pointer

    private:
        std::unique_ptr<httplib::Client> client; // HTTP client instance
        AppServer* server_ptr; // Use raw pointer to avoid circular dependency
        std::string username; // username (used for authentication)
        std::string hostname; // host + port (used for authentication)
        Config::ClientStatus client_status; // Generate the client with the server information
};

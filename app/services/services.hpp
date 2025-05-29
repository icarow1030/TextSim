#pragma once
#include "../server/AppServer.hpp"
#include "../cryptography/rsa.hpp"
#include <memory>
#include <iostream>
#include <nlohmann/json.hpp> // For JSON parsing
#include "../chat/chat.hpp"
#include "../config/config.hpp"
#include <httplib.h>
#include <gmp.h>

// - Start Server Mode
// - Start Client Mode (Chat)
// - Change Destination Port
// - Change Current Port
// - Change Connection
// - Edit Username
// - Generate RSA Keys

// Function to change the current server port
namespace Services {
    void connectToTargetAsConfig();
    void connectToTargetDirect(int port);
    void connectToTargetDirect(int port, const std::string& host);
    void disconnectFromServer(AppServer& server);
    void startServerMode(AppServer& server); // Done
    void stopServerMode(AppServer& server); // Done
    void startClientMode();
    void checkServerStatus(AppServer& server); // Done
    void changeCurrentPort(AppServer& server); // Done
    void changeTargetPort(AppServer& server);
    void editUsername(Chat& chat);
    void generateRSAKeys();
    void showPortsInfo(AppServer& server);
    void generateUserId();
    void clearTerminal();
}

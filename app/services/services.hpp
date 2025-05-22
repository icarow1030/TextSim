#pragma once
#include "../server/AppServer.hpp"

// - Start Server Mode
// - Start Client Mode (Chat)
// - Change Destination Port
// - Change Current Port
// - Change Connection
// - Edit Username
// - Generate RSA Keys

// Function to change the current server port
namespace Services {
    void connectToServerAsConfig();
    void connectToServerDirect(int port);
    void disconnectFromServer(AppServer& server);
    void startServerMode(AppServer& server);
    void stopServerMode(AppServer& server);
    void startClientMode();
    void checkConnection(AppServer& server);
    void changeCurrentPort(AppServer& server);
    void changeTargetPort(AppServer& server);
    void editUsername();
    void generateRSAKeys();
}

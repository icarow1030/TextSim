#include "services.hpp"

// Function to change the current server port
void Services::changeCurrentPort(AppServer& server) {
    std::cout << "Current configured port: " << Config::Server::PORT;
    if(Config::Server::PORT == Config::Default::PORT) {
        std::cout << " (Default)" << std::endl;
    } else {
        std::cout << std::endl;
    }
    std::cout << "Server port: ";
    if(server.status() == Config::Connection::CONNECTED) {
        std::cout << server.getPort() << std::endl;
    } else {
        std::cout << "Server not started." << std::endl;
    }
    std::cout << "Enter new port: ";
    std::cin >> Config::Server::PORT;
    if(server.status() == Config::Connection::CONNECTED && server.getPort() != Config::Server::PORT) {
        std::cout << "Server port is different from current port." << std::endl;
        char choice;
        do {
            std::cout << "Update server port? (y/n): ";
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                server.updatePort();
                std::cout << "Server port updated to: " << server.getPort() << std::endl;
            } else {
                std::cout << "Server port not updated." << std::endl;
            }
        } while(choice != 'y' && choice != 'n');
    }
    std::cout << "Port updated: " << Config::Server::PORT << std::endl;
}
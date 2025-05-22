#include "services.hpp"
#include "../config/config.hpp"
#include <httplib.h>


void Services::checkConnection(AppServer& server) {
    std::cout << "Checking connection..." << std::endl;
    httplib::Client client(Config::Server::HOST, server.getPort());

    if(server.status() == Config::ServerStatus::ONLINE) {
        std::cout << "Connected to server at port: " << server.getPort() << std::endl;
        if(auto res = client.Get("/checkConnection")) {
            if(res->status == 200) {
                std::cout << "Server is running." << std::endl;
            } else {
                std::cout << "Server is not responding, but status is ONLINE." << std::endl;
            }
        } else {
            std::cout << "Error connecting to server " << std::endl;
        }
    } else {
        std::cout << "Not connected to server." << std::endl;
        if(auto res = client.Get("/checkConnection")) {
            if(res->status == 200) {
                std::cout << "Server is running, but status is OFFLINE." << std::endl;
            } else {
                std::cout << "Server is not responding." << std::endl;
            }
        } else {
            std::cout << "Error connecting to server " << std::endl;
        }
    }
}

void Services::startServerMode(AppServer& server) {
    std::cout << "Starting server mode..." << std::endl;
    try {
        server.start();
        std::cout << "Server started on port: " << server.getPort() << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Error starting server: " << e.what() << std::endl;
    }
}

void Services::stopServerMode(AppServer& server) {
    std::cout << "Stopping server mode..." << std::endl;
    try {
        server.stop();
        std::cout << "Server stopped." << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Error stopping server: " << e.what() << std::endl;
    }
}

// Function to change the current server port
void Services::changeCurrentPort(AppServer& server) {
    std::cout << "Current configured port: " << Config::Server::PORT;
    if(Config::Server::PORT == Config::Default::PORT) {
        std::cout << " (Default)" << std::endl;
    } else {
        std::cout << std::endl;
    }
    std::cout << "Server port: ";
    if(server.status() == Config::ServerStatus::ONLINE) {
        std::cout << server.getPort() << std::endl;
    } else {
        std::cout << "Server not started." << std::endl;
    }
    std::cout << "Enter new port: ";
    std::cin >> Config::Server::PORT;
    if(server.status() == Config::ServerStatus::ONLINE && server.getPort() != Config::Server::PORT) {
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
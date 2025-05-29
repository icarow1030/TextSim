#include "AppClient.hpp"


AppClient::AppClient() {}

void AppClient::handshakeSend(const std::string target_host, const int target_port) {
    json message = {
        {"username", Config::User::USERNAME},
        {"user_id", Config::User::USER_ID},
        {"host", Config::Server::HOST},
        {"port", Config::Server::PORT},
        {"pub_key", {
            {"n", Config::User::PUBLIC_KEY.n.get_str()},
            {"e", Config::User::PUBLIC_KEY.e.get_str()}
        }}
    };
    httplib::Client client(target_host, target_port);
    std::string body = message.dump();
    if (body.empty()) {
        std::cerr << "Handshake message body is empty. Cannot send." << std::endl;
        return;
    }
    httplib::Headers headers = { {"Content-Type", "application/json"} };
    auto res = client.Post("/handshake", headers, body, "application/json");
    if (res && res->status == 200) {
        auto response_json = json::parse(res->body);
        if (response_json.contains("status") && response_json["status"] == "success" && !response_json["pub_key"].empty()) {
            Config::Server::TARGET_PUBLIC_KEY.n = response_json["pub_key"]["n"].get<std::string>();
            Config::Server::TARGET_PUBLIC_KEY.e = response_json["pub_key"]["e"].get<std::string>();
            std::cout << "Handshake successful with server at " << target_host << ":" << target_port << " [" << response_json["username"] << "]" << std::endl;
            client_status = Config::ClientStatus::ONLINE;
        } else {
            std::cerr << "Handshake failed: " << response_json.value("error", "Unknown error") << std::endl;
            client_status = Config::ClientStatus::ERROR_;
        }
    } else if(res) {
        std::cerr << "Handshake failed. Status: " << res->status << std::endl;
        client_status = Config::ClientStatus::ERROR_;
    } else {
        std::cerr << "Handshake failed. No response received." << std::endl;
        client_status = Config::ClientStatus::ERROR_;
    }
}

httplib::Result AppClient::sendMessage(json message) {
    httplib::Client client(Config::Server::TARGET_HOST, Config::Server::TARGER_PORT);
    std::string body = message.dump();
    if (body.empty()) {
        std::cerr << "Message body is empty. Cannot send." << std::endl;
        return httplib::Result(nullptr, httplib::Error::Unknown);
    }
    httplib::Headers headers = { {"Content-Type", "application/json"} };
    auto res = client.Post("/message", headers, body, "application/json");
    if (!res || res->status != 200) {
        std::cerr << "Failed to send message. Status: " << (res ? std::to_string(res->status) : "No response") << std::endl;
    }
    return res;
}


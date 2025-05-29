
#include "AppServer.hpp"


using namespace httplib;

AppServer::AppServer(Chat& chatRef)
    : server_status(Config::ServerStatus::OFFLINE),
      port(Config::Server::PORT),
      host(Config::Server::HOST),
      chat(chatRef) // inicializa a referência
{}

int AppServer::getPort() const {
    return port;
}

void AppServer::updatePort(int new_port) {
    if (new_port <= 0 || new_port > 65535) {
        throw std::invalid_argument("Port number must be between 1 and 65535.");
    }
    port = new_port;
    
}

void AppServer::setupRoutes() {
    server.Get("/", [](const Request&, Response& res) {
        res.set_content("Welcome to the server!", "text/plain");
    });
    server.Get("/checkConnection", [](const Request&, Response& res) {
        res.set_content("Server is running", "text/plain");
    });
    server.Post("/message", [this](const Request& req, Response& res) {
        handleMessage(req);
        res.set_content("Message received", "text/plain");
    });
    server.Post("/handshake", [this](const Request& req, Response& res) {
        try {
            auto json_body = nlohmann::json::parse(req.body);
            if (json_body["username"].empty() || json_body["pub_key"].empty())
                throw std::invalid_argument("Invalid handshake data");
            Config::Server::TARGET_USER_ID = json_body["user_id"].get<std::string>();
            Config::Server::TARGET_USERNAME = json_body["username"].get<std::string>();
            Config::Server::TARGER_PORT = json_body["port"].get<int>();
            Config::Server::TARGET_PUBLIC_KEY.n = json_body["pub_key"]["n"].get<std::string>();
            Config::Server::TARGET_PUBLIC_KEY.e = json_body["pub_key"]["e"].get<std::string>();
            json response = {
                {"username", Config::User::USERNAME},
                {"status", "success"},
                {"pub_key", {
                    {"n", Config::User::PUBLIC_KEY.n.get_str()},
                    {"e", Config::User::PUBLIC_KEY.e.get_str()}
                }}
            };
            res.set_content(response.dump(), "application/json");
        } catch(const std::exception& e) {
            res.status = 400;
            res.set_content(nlohmann::json{{"error", e.what()}}.dump(), "application/json");
        }
    });
}

void AppServer::start() {
    try {
        setupRoutes();
        server_thread = std::thread([this]() {
            try {
                server.listen(host, port);
            } catch(const std::exception& e) {
                std::cerr << "Error in server thread: " << e.what() << std::endl;
                server_status = Config::ServerStatus::OFFLINE;
            }
        });
        server_status = Config::ServerStatus::ONLINE;
        server_thread.detach();
    } catch(const std::exception& e) {
        std::cerr << "Error starting server: " << e.what() << std::endl;
        server_status = Config::ServerStatus::ERROR_;
    }
}

void AppServer::stop() {
    server.stop();
    server_status = Config::ServerStatus::OFFLINE;
}

Config::ServerStatus AppServer::status() {
    return server_status;
}

void AppServer::handleMessage(const Request& req) {
    try {
        auto json_body = nlohmann::json::parse(req.body);
        auto decrypt_field = [this](const std::string& encrypted_b64) {
            if (encrypted_b64.empty()) throw std::runtime_error("String Base64 vazia recebida");
            std::vector<unsigned char> decoded = Base64::decode(encrypted_b64);
            if (decoded.empty()) throw std::runtime_error("Dados decodificados estão vazios");
            mpz_class encrypted_mpz;
            mpz_import(encrypted_mpz.get_mpz_t(), decoded.size(), 1, sizeof(decoded[0]), 0, 0, decoded.data());
            std::string decrypted = RSA::decrypt(encrypted_mpz, Config::User::PRIVATE_KEY);
            if (decrypted.empty()) throw std::runtime_error("Falha na descriptografia - resultado vazio");
            return decrypted;
        };
        json decrypted_message = {
            {"user_id", decrypt_field(json_body["user_id"].get<std::string>())},
            {"username", decrypt_field(json_body["username"].get<std::string>())},
            {"content", decrypt_field(json_body["content"].get<std::string>())}
        };
        std::string string_to_hash = decrypted_message["user_id"].get<std::string>() + ":" +
                                     decrypted_message["username"].get<std::string>() + ":" +
                                     decrypted_message["content"].get<std::string>();
        std::string hash_calculated = SHA256::hash(string_to_hash);
        std::string received_hash = decrypt_field(json_body["hash"].get<std::string>());
        if (hash_calculated != received_hash)
            throw std::runtime_error("Hash mismatch. Message integrity compromised.");
        if(decrypted_message["user_id"] != Config::User::USER_ID)
            chat.addMessage(decrypted_message);
    } catch(const std::exception& e) {
        std::cerr << "Error handling message: " << e.what() << std::endl;
    }
}
cmake_minimum_required(VERSION 3.10)

project(TextSim)

set(CMAKE_CXX_STANDARD 17)

find_package(httplib CONFIG REQUIRED)

# Adicione o caminho de includes e libs do vcpkg (ajuste se necessário)
include_directories("C:/MY-SPACE/vcpkg-Gerenciador-de-Pacotes-cpp/vcpkg/installed/x64-windows/include")
link_directories("C:/MY-SPACE/vcpkg-Gerenciador-de-Pacotes-cpp/vcpkg/installed/x64-windows/lib")

add_executable(TextSim 
    app/main.cpp
    app/chat/chat.cpp
    app/client/AppClient.cpp
    app/server/AppServer.cpp
    app/services/services.cpp
    app/config/config.cpp
    app/cryptography/rsa.cpp
    app/cryptography/sha256.cpp
    app/cryptography/base64.cpp
)

target_link_libraries(TextSim PRIVATE
    httplib::httplib
    gmp
    gmpxx
)

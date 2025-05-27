cmake_minimum_required(VERSION 3.10)
project(TextSim)

set(CMAKE_CXX_STANDARD 17)

# Adiciona o executável principal
add_executable(TextSim 
    app/main.cpp
    app/chat/chat.cpp
    app/client/AppClient.cpp
    app/server/AppServer.cpp
    app/services/services.cpp
)

# Habilita o sistema de pacotes do vcpkg
find_package(httplib CONFIG REQUIRED)

target_link_libraries(TextSim PRIVATE httplib::httplib)

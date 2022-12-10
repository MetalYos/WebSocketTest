cmake_minimum_required(VERSION 3.13)

project(WS_Client VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory("libs/IXWebSocket")

add_executable(WS_Client main.cpp WebSocketClient.cpp)

target_link_libraries(WS_Client PUBLIC 
    ixwebsocket
    )

target_include_directories(WS_Client PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/libs/IXWebSocket/ixwebsocket"
    )

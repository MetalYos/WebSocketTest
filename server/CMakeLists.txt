cmake_minimum_required(VERSION 3.13)

project(WS_Server VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory("libs/IXWebSocket")

add_executable(WS_Server main.cpp WebSocketServer.cpp)

target_link_libraries(WS_Server PUBLIC 
    ixwebsocket
    )

target_include_directories(WS_Server PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/libs/IXWebSocket/ixwebsocket"
    )

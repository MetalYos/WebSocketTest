#pragma once

#include "ixwebsocket/IXWebSocketServer.h"
#include <string>

class WsServer {
public:
    struct Config {
        int port;
        std::string hostname;
        ix::SocketTLSOptions tlsOptions;
        bool ipv6;
        bool diablePerMessageDeflate;
        bool disablePong;
        std::string httpHeaderAuthorization;

        int backlog;
        int maxConnections;
        int handshakeTimeoutSecs;
    };

public:
    WsServer(const Config& initConfig);
    ~WsServer();

    bool Listen();
    void Start();
    void Wait();
    void Stop();

private:
    Config config;
    ix::WebSocketServer server;
};

#include "WebSocketServer.h"
#include "ixwebsocket/IXConnectionState.h"
#include "ixwebsocket/IXWebSocket.h"
#include <iostream>

WsServer::WsServer(const Config& initConfig) : 
    config(initConfig), 
    server(initConfig.port, 
        initConfig.hostname,
        initConfig.backlog,
        initConfig.maxConnections,
        initConfig.handshakeTimeoutSecs,
        initConfig.ipv6 ? AF_INET6 : AF_INET)
{
    server.setTLSOptions(initConfig.tlsOptions);
    if (initConfig.diablePerMessageDeflate)
        server.disablePerMessageDeflate();
    if (initConfig.disablePong)
        server.disablePong();

    std::string httpHeaderAuthorization = initConfig.httpHeaderAuthorization;
    server.setOnClientMessageCallback(
        [httpHeaderAuthorization](std::shared_ptr<ix::ConnectionState> connectionState,
                                         ix::WebSocket& webSocket,
                                         const ix::WebSocketMessagePtr& msg) {
        auto remoteIp = connectionState->getRemoteIp();
        if (msg->type == ix::WebSocketMessageType::Open)
        {
            std::cout << "New connection" << std::endl;
            std::cout << "remote ip: " << remoteIp << std::endl;
            std::cout << "id: " << connectionState->getId() << std::endl;
            std::cout << "Uri: " << msg->openInfo.uri << std::endl;
            std::cout << "Headers:" << std::endl;
            for (auto it : msg->openInfo.headers)
                std::cout << it.first << ": " << it.second << std::endl;

            if (!httpHeaderAuthorization.empty()) {
                auto authorization = msg->openInfo.headers["Authorization"];
                if (authorization != httpHeaderAuthorization)
                    webSocket.close(4001, "Permission denied");
                else
                    webSocket.sendText("Authorization suceeded!");
            }
        }
        else if (msg->type == ix::WebSocketMessageType::Close) {
            std::cout << "Closed connection: client id " <<
                         connectionState->getId() <<
                         "code " <<
                         msg->closeInfo.code <<
                         "reason " << 
                         msg->closeInfo.reason << std::endl;
        }
        else if (msg->type == ix::WebSocketMessageType::Error) {
            std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
            std::cout << "#retries: " << msg->errorInfo.retries << std::endl;
            std::cout << "Wait time(ms): " << msg->errorInfo.wait_time << std::endl;
            std::cout << "HTTP Status: " << msg->errorInfo.http_status << std::endl;
        }
        else if (msg->type == ix::WebSocketMessageType::Message) {
            std::cout << "Server: received message: " << msg->str << std::endl;
            webSocket.send(msg->str, msg->binary);
        }
    });
}

WsServer::~WsServer() {
    server.stop();
}

bool WsServer::Listen() {
    auto res = server.listen();
    if (!res.first) {
        std::cout << "Listen Error: " << res.second << std::endl;
        return false;
    }
    return true;
}

void WsServer::Start() {
    server.start();
}

void WsServer::Wait() {
    server.wait();
}

void WsServer::Stop() {
    server.stop();
}


//#include "WS_ServerConfig.h"
#include "WebSocketServer.h"

int main() {
    WsServer::Config initConfig;
    initConfig.port = 8008;
    initConfig.hostname = "127.0.0.1";
    initConfig.ipv6 = false;
    initConfig.diablePerMessageDeflate = true;
    initConfig.disablePong = false;
    initConfig.backlog = ix::SocketServer::kDefaultTcpBacklog;
    initConfig.maxConnections = ix::SocketServer::kDefaultMaxConnections;
    initConfig.handshakeTimeoutSecs = ix::WebSocketServer::kDefaultHandShakeTimeoutSecs;
    WsServer server(initConfig);

    server.Listen();
    server.Start();
    server.Wait();

    return 1;
}

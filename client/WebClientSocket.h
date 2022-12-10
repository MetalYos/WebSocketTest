#pragma once

#include "ixwebsocket/IXWebSocket.h"
#include <string>

class WsClient {
public:
    struct Config {
        std::string url;
        bool disableAutomaticReconnection;
        bool disablePerMessageDeflate;
        unsigned int maxWaitBetweenReconnectionRetries;
        ix::SocketTLSOptions tlsOptions;
        std::string subprotocol;
        int pingIntervalSecs;
        bool decompressGzipMessages;
    };

public:
    WsClient(const Config& initConfig);
    ~WsClient();

    void Start();
    void SendText(const std::string& text);
    void SendBinary(const char* data);
    void Stop();

    bool IsConnectionOpened() const;
    int GetTotalBytesReceived() const;
    int GetTotalBytesSent() const;

private:
    ix::WebSocket webSocket;
    Config config;
    bool isConnectionOpened;

    std::atomic<int> receivedBytes;
    std::atomic<int>sentBytes;
};


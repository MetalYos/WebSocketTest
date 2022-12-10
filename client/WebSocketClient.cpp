#include "WebClientSocket.h"
#include "ixwebsocket/IXWebSocketHttpHeaders.h"
#include "ixwebsocket/IXWebSocketMessageType.h"
#include "ixwebsocket/IXGzipCodec.h"
#include <iostream>
#include <sstream>

WsClient::WsClient(const Config& initConfig) {
    config = initConfig;

    if (initConfig.disableAutomaticReconnection)
        webSocket.disableAutomaticReconnection();
    webSocket.setMaxWaitBetweenReconnectionRetries(initConfig.maxWaitBetweenReconnectionRetries);
    webSocket.setTLSOptions(initConfig.tlsOptions);
    webSocket.setPingInterval(initConfig.pingIntervalSecs);

    if (!initConfig.subprotocol.empty())
        webSocket.addSubProtocol(initConfig.subprotocol);

    receivedBytes = 0;
    sentBytes = 0;

    webSocket.setTrafficTrackerCallback([this](int size, bool incoming) {
        if (incoming)
        {
            receivedBytes += size;
        }
        else
        {
            sentBytes += size;
        }
    });
}

WsClient::~WsClient() {
    Stop();
}

void WsClient::Start() {
    webSocket.setUrl(config.url);

    if (config.disablePerMessageDeflate)
        webSocket.disablePerMessageDeflate();
    else {
        ix::WebSocketPerMessageDeflateOptions webSocketPerMessageDeflateOptions(
            true, false, false, 15, 15);
        webSocket.setPerMessageDeflateOptions(webSocketPerMessageDeflateOptions);
    }

    webSocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
            std::string payload = msg->str;
            if (config.decompressGzipMessages) {
                std::string decompressedBytes;
                if (ix::gzipDecompress(payload, decompressedBytes))
                    payload = decompressedBytes;
                else
                    std::cout << "Error decompressing: " << payload << std::endl;
            }
            std::cout << "Client: recevied message: " << payload << std::endl;
        }
        else if (msg->type == ix::WebSocketMessageType::Open) {
            isConnectionOpened = true;
        }
        else if (msg->type == ix::WebSocketMessageType::Close) {
            isConnectionOpened = false;
        }
        else if (msg->type == ix::WebSocketMessageType::Error) {
            std::stringstream ss;
            ss << "Error: "         << msg->errorInfo.reason      << std::endl;
            ss << "#retries: "      << msg->errorInfo.retries     << std::endl;
            ss << "Wait time(ms): " << msg->errorInfo.wait_time   << std::endl;
            ss << "HTTP Status: "   << msg->errorInfo.http_status << std::endl;
            std::cout << ss.str() << std::endl;
        }
        else if (msg->type == ix::WebSocketMessageType::Fragment)
            std::cout << "Received message fragment" << std::endl;
        else if (msg->type == ix::WebSocketMessageType::Ping)
            std::cout << "Received ping" << std::endl;
        else if (msg->type == ix::WebSocketMessageType::Pong)
            std::cout << "Received pong" << std::endl;
        else
            std::cout << "Invalid ix::WebSocketMessageType" << std::endl;
    });

    webSocket.start();
}

void WsClient::SendText(const std::string& text) {
    webSocket.sendText(text);
}

void WsClient::SendBinary(const char* data) {
    webSocket.sendBinary(data);
}

void WsClient::Stop() {
    webSocket.stop();
}

bool WsClient::IsConnectionOpened() const {
    return isConnectionOpened;
}

int WsClient::GetTotalBytesReceived() const {
    return receivedBytes;
}

int WsClient::GetTotalBytesSent() const {
    return sentBytes;
}


//#include "WS_ClientConfig.h"
#include <iostream>
#include <string>
#include <sstream>
#include "WebClientSocket.h"

int main() {
    WsClient::Config initConfig {
        "ws://localhost:8008",
        false,
        true,
        1000,
        ix::SocketTLSOptions(),
        "",
        45,
        false
    };
    WsClient client(initConfig);
    client.Start();

    bool sendText = true;
    bool sendBinary = false;
    while (true) {
        if (client.IsConnectionOpened()) {
            if (sendText) {
                client.SendText("Hello World!");
                sendText = false;
                sendBinary = true;
            }
            else if (sendBinary) {
                client.SendBinary("Some serialized binary data!");
                sendBinary = false;
            }
        }
    }
    client.Stop();

    return 1;
}

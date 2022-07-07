// File: test_server.cpp
// Author: Caleb Johnson-Cantrell

#include <iostream>

#include "easy_net.hpp"

using namespace easy_net;

enum class CustomMessageType { CMT_INVALID,
                               CMT_ECHO };

class Server : public ServerInterface<CustomMessageType> {
  public:
    Server(uint16_t port) :
        ServerInterface(port) {}

    void on_message_receive(Message<CustomMessageType> msg, Connection<CustomMessageType>::Ptr client) override {
        std::cout << msg << "\n";
        client->send_message(msg);
    }

    bool on_client_connect(Endpoint client_endpoint) override {
        return true;
    }
};

int main(int argc, char **argv) {
    Server s(5000);
    s.start();
    std::cin.get();
}

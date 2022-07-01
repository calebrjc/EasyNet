// File: test_server.cpp
// Author: Caleb Johnson-Cantrell

#include <iostream>

#include "easy_net.hpp"

using namespace easy_net;

enum class CustomMessageType { PING };

class Server : public ServerInterface<CustomMessageType> {
  public:
    Server(uint16_t port) :
        ServerInterface(port) {}

    void on_message_receive(Message<CustomMessageType> message, Connection<CustomMessageType>::Ptr client) override {}

    bool on_client_connect(Endpoint client_endpoint) override {
        return true;
    }
};

int main(int argc, char **argv) {
    Server s(5000);
    s.start();
    std::cin.get();
}

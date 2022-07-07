// File: test_client.cpp
// Author: Caleb Johnson-Cantrell

#include <iostream>
#include <string>

#include "easy_net.hpp"

using namespace easy_net;

enum class CustomMessageType { CMT_INVALID,
                               CMT_ECHO };

TSQueue<Message<CustomMessageType>> msg_queue;

class Client : public ClientInterface<CustomMessageType> {
  public:
    Client(const std::string &host, const std::string &port) :
        ClientInterface(host, port) {}

    void on_message_receive(Message<CustomMessageType> msg) override {
        msg_queue.push(msg);
    }
};

int main(int argc, char **argv) {
    Client c("127.0.0.1", "5000");
    c.connect();

    Message<CustomMessageType> m;
    m.header.type = CustomMessageType::CMT_ECHO;
    // m << 1;
    c.send_message(m);

    msg_queue.wait();
    auto msg = msg_queue.pop();
    std::cout << msg << "\n";
}

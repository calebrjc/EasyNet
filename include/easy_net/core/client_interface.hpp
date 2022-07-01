// File: easy_net/core/client_interface.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <string>

#include "message.hpp"

namespace easy_net {
    template<typename T>
    class ClientInterface {
      public:
        ClientInterface(const std::string &host, const std::string &port_or_service);
        virtual ~ClientInterface();

        bool connect();
        void disconnect();

        void send_message(Message<T> message);
    };
}  // namespace easy_net

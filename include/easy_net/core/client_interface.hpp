// File: easy_net/core/client_interface.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <string>

#include "easy_net/core/asio_stuff.hpp"
#include "easy_net/core/connection.hpp"
#include "easy_net/core/message.hpp"

namespace easy_net {
    template<typename T>
    class ClientInterface {
      public:
        ClientInterface(const std::string &host, const std::string &port_or_service);
        virtual ~ClientInterface();

        bool connect();
        void disconnect();

        void send_message(Message<T> message);

      protected:
        virtual void on_message_receive(Message<T> msg) = 0;
        virtual void on_connect();
        virtual void on_disconnect();

      private:
        std::string host_;
        std::string port_or_service_;

        asio::io_context io_context_;
        std::thread context_thread_;

        typename Connection<T>::Ptr connection_;
    };
}  // namespace easy_net

#include "easy_net/core/impl/client_interface.ipp"

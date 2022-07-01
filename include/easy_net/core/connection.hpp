// File: easy_net/core/connection.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <functional>
#include <memory>

#include "easy_net/core/asio_stuff.hpp"
#include "easy_net/core/endpoint.hpp"
#include "easy_net/core/message.hpp"
#include "easy_net/core/tsqueue.hpp"

using asio::ip::tcp;

namespace easy_net {
    template<typename T>
    class Connection : std::enable_shared_from_this<Connection<T>> {
      public:
        typedef std::shared_ptr<Connection<T>> Ptr;
        typedef std::function<void(Message<T>, Ptr)> ReceiveCallback;
        typedef std::function<void(Ptr)> DisconnectCallback;

        Connection(tcp::socket socket, ReceiveCallback on_recv, DisconnectCallback on_disconnect, uint32_t id = 0);
        static Ptr create(tcp::socket socket, ReceiveCallback on_recv, DisconnectCallback on_disconnect, uint32_t id = 0);
        virtual ~Connection();

        void open();
        void close();

        bool is_connected() const;

        Endpoint local_endpoint();
        Endpoint remote_endpoint();

        uint32_t id() const;

        // void set_receive_callback(ReceiveCallback func);
        // void set_disconnect_callback(DisconnectCallback func);

        void send_message(Message<T> msg);

        const tcp::socket &socket();

      private:
        void send_message_(Message<T> msg);
        void recv_message_();

        tcp::socket socket_;

        ReceiveCallback on_message_receive_;
        DisconnectCallback on_disconnect_;

        uint32_t id_;

        TSQueue<Message<T>> out_queue_;
        Message<T> incoming_message_;
    };
}  // namespace easy_net

#include "easy_net/core/impl/connection.ipp"

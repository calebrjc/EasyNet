// File: easy_net/core/server_interface.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <thread>
#include <unordered_set>
#include <vector>

#include <cstdint>

#include "easy_net/core/asio_stuff.hpp"
#include "easy_net/core/connection.hpp"
#include "easy_net/core/endpoint.hpp"
#include "easy_net/core/message.hpp"

namespace easy_net {
    template<typename T>
    class ServerInterface {
      public:
        // Constructs a server that will be bound to listen for incoming connections on the specified port
        ServerInterface(uint16_t port);

        // Destructs this server
        virtual ~ServerInterface();

        // Begins this server's opzeration, prompting it to accept incoming connections and respond
        // to incoming messages
        bool start();

        // Stops this server's operation, prompting it to stop accepting incoming connections and
        // responding to incoming messages
        void stop();

        // Send a message to all clients, except for the one referenced by ignore
        void send_broadcast(Message<T> msg, typename Connection<T>::Ptr ignore = nullptr);

      protected:
        // Called when this server receives a message from a particular client
        virtual void on_message_receive(Message<T> msg, typename Connection<T>::Ptr client) = 0;

        // Called when a potential client is attempting to connect to this server. Return true if
        // the client should move on to validating this client, or false to reject the connection
        // attempt
        //
        // A call to this function does not guarantee that the remote end of the connection is a
        // legitimate client application. This function should be useed for preliminary evaluation
        // (e.g. IP-level filtering) and NOT for communication with the potential client
        virtual bool on_client_connect(Endpoint client_endpoint) = 0;

        // Called when a client has been validated as a legitimate client application
        //
        // Optional; override if there is some action that the server should take for every client
        // after it has been validated
        virtual void on_client_validated(typename Connection<T>::Ptr client);

        // Called when a client is assumed to have been disconnected
        //
        // Optional; override if there is some cleanup task that the server should do when a client
        // disconnects
        virtual void on_client_disconnect(typename Connection<T>::Ptr client);

      private:
        void await_connection_();

        asio::io_context io_context_;
        std::thread context_thread_;
        asio::ip::tcp::acceptor acceptor_;

        std::vector<typename Connection<T>::Ptr> connections_;

        uint32_t next_id = 1;
    };
}  // namespace easy_net

#include "easy_net/core/impl/server_interface.ipp"

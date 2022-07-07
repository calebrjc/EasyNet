// File: easy_net/core/impl/server_interface.ipp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <algorithm>

#include "easy_net/core/server_interface.hpp"
#include "easy_net/util/logging.hpp"

using asio::ip::tcp;

namespace easy_net {
    template<typename T>
    ServerInterface<T>::ServerInterface(uint16_t port) :
        acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {}

    template<typename T>
    ServerInterface<T>::~ServerInterface() { stop(); }

    template<typename T>
    bool ServerInterface<T>::start() {
        // Begin accepting connections
        await_connection_();

        try {
            context_thread_ = std::thread([this]() { io_context_.run(); });
        } catch (std::exception &error) {
            EZN_CRITICAL("SERVER: Server error: {}", error.what());
            return false;
        }

        EZN_INFO("SERVER: Listening for connections at {}", acceptor_.local_endpoint());
        return true;
    }

    template<typename T>
    void ServerInterface<T>::stop() {
        io_context_.stop();
        if (context_thread_.joinable()) context_thread_.join();

        EZN_INFO("SERVER: Stopped");
    }

    template<typename T>
    void ServerInterface<T>::send_broadcast(Message<T> msg, typename Connection<T>::Ptr ignore) {
        for (auto &client : connections_) {
            if (client && client->is_connected() && client != ignore) {
                client->send_message(msg);
            }
        }
    }

    template<typename T>
    void ServerInterface<T>::on_client_validated(typename Connection<T>::Ptr client) {}

    template<typename T>
    void ServerInterface<T>::on_client_disconnect(typename Connection<T>::Ptr client) {}

    template<typename T>
    void ServerInterface<T>::await_connection_() {
        // Wait for a connection attempt on our server's acceptor socket
        acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
            if (ec) {
                EZN_CRITICAL("SERVER: Error accepting connection: {}", ec.message());
                stop();
                return;
            }

            // Someone connected to the server
            auto remote_addr = socket.remote_endpoint();
            EZN_INFO("SERVER: New connection request from {}", remote_addr);

            // Create the new connection
            auto recieve_handler = [this](Message<T> msg, typename Connection<T>::Ptr client) {
                on_message_receive(msg, client);
            };

            auto disconnect_handler = [this](typename Connection<T>::Ptr client) {
                on_client_disconnect(client);
            };

            auto client_endpoint = Endpoint::from_asio_endpoint(remote_addr);

            // Allow the user to veto the connection
            bool connection_allowed = on_client_connect(client_endpoint);
            if (connection_allowed) {
                // Validate the connection
                // TODO(Caleb): Add socket-level transaction for host to validate client

                // Issue an ID to the connection
                std::error_code ec;
                uint32_t conn_id     = next_id++;
                uint32_t net_conn_id = asio::detail::socket_ops::host_to_network_long(conn_id);
                asio::write(socket, asio::buffer(&net_conn_id, sizeof(uint32_t)), asio::transfer_exactly(sizeof(uint32_t)), ec);
                if (ec) {
                    EZN_ERROR("{:06d}: Connection rejected; could not allocate id; {}", ec.message());
                } else {
                    // Add the new connection to the container of connections
                    auto new_connection = Connection<T>::create(std::move(socket), recieve_handler, disconnect_handler, conn_id);
                    connections_.push_back(new_connection);

                    // Open the connection
                    connections_.back()->open();
                    EZN_INFO("{:06d}: Connection from {} accepted", new_connection->id(), new_connection->remote_endpoint());
                }
            }

            await_connection_();
        });
    }
}  // namespace easy_net

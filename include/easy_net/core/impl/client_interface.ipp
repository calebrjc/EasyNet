// File: easy_net/core/impl/client_interface.ipp
// Author: Caleb Johnson-Cantrell

#pragma once

#include "easy_net/core/client_interface.hpp"
#include "easy_net/util/logging.hpp"

using asio::ip::tcp;

namespace easy_net {
    template<typename T>
    ClientInterface<T>::ClientInterface(const std::string &host, const std::string &port_or_service) :
        host_(host), port_or_service_(port_or_service) {}

    template<typename T>
    ClientInterface<T>::~ClientInterface() { disconnect(); }

    template<typename T>
    bool ClientInterface<T>::connect() {
        std::error_code ec;

        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host_, port_or_service_, ec);
        if (ec) {
            EZN_CRITICAL("CLIENT: Error resolving address; {}", ec.message());
            return false;
        }

        // Connect to server
        tcp::socket socket(io_context_);
        asio::connect(socket, endpoints, ec);
        if (ec) {
            EZN_CRITICAL("CLIENT: Error connecting to server; {}", ec.message());
            return false;
        }

        // Information exchange
        uint32_t id = 0;
        asio::read(socket, asio::buffer(&id, sizeof(uint32_t)), asio::transfer_exactly(sizeof(uint32_t)), ec);
        if (ec) {
            EZN_CRITICAL("CLIENT: Error connecting to server; {}", ec.message());
            return false;
        }
        id = asio::detail::socket_ops::network_to_host_long(id);

        // Hand socket off to connection
        auto receive_handler = [this](Message<T> msg, typename Connection<T>::Ptr connection) {
            this->on_message_receive(msg);
        };

        auto disconnect_handler = [this](typename Connection<T>::Ptr client) {
            this->on_disconnect();
        };

        connection_ = Connection<T>::create(std::move(socket), receive_handler, disconnect_handler, id);

        // Open connection
        connection_->open();
        try {
            context_thread_ = std::thread([this]() { io_context_.run(); });
        } catch (std::exception &error) {
            EZN_CRITICAL("CLIENT: Client error: {}", error.what());
            return false;
        }

        EZN_INFO("CLIENT: Connection established; id = {}", connection_->id());
        return true;
    }

    template<typename T>
    void ClientInterface<T>::disconnect() {
        if (!connection_->is_connected()) {
            return;
        }

        connection_->close();
        io_context_.stop();
        if (context_thread_.joinable()) context_thread_.join();

        EZN_INFO("CLIENT: Stopped");
    }

    template<typename T>
    void ClientInterface<T>::send_message(Message<T> msg) {
        if (!connection_->is_connected()) {
            return;
        }

        connection_->send_message(msg);
    }

    template<typename T>
    void ClientInterface<T>::on_connect() {}

    template<typename T>
    void ClientInterface<T>::on_disconnect() {}
}  // namespace easy_net

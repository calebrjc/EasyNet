// File: easy_net/core/impl/connection.ipp
// Author: Caleb Johnson-Cantrell

#pragma once

#include "easy_net/core/connection.hpp"

#include "easy_net/util/asio_buffer_sequences.hpp"
#include "easy_net/util/logging.hpp"

namespace easy_net {
    template<typename T>
    Connection<T>::Connection(tcp::socket socket, ReceiveCallback on_recv, DisconnectCallback on_disconnect, uint32_t id) :
        socket_(std::move(socket)), on_message_receive_(on_recv), on_disconnect_(on_disconnect), id_(id) {}

    template<typename T>
    typename Connection<T>::Ptr Connection<T>::create(tcp::socket socket, ReceiveCallback on_recv, DisconnectCallback on_disconnect, uint32_t id) {
        return std::make_shared<Connection<T>>(std::move(socket), on_recv, on_disconnect, id);
    }

    template<typename T>
    Connection<T>::~Connection() { close(); }

    template<typename T>
    void Connection<T>::open() {
        if (is_connected()) {
            return;
        }

        recv_message_();
        EZN_INFO("{:06d}: Connection opened", id_);
    }

    template<typename T>
    void Connection<T>::close() {
        if (!is_connected()) {
            return;
        }

        asio::post(socket_.get_executor(), [this]() {
            this->socket_.close();
            on_disconnect_(this->shared_from_this());
            EZN_INFO("{:06d}: Connection closed", id_);
        });
    }

    template<typename T>
    bool Connection<T>::is_connected() const {
        return socket_.is_open();
    }

    template<typename T>
    Endpoint Connection<T>::local_endpoint() {
        return Endpoint::from_asio_endpoint(socket_.local_endpoint());
    }

    template<typename T>
    Endpoint Connection<T>::remote_endpoint() {
        return Endpoint::from_asio_endpoint(socket_.remote_endpoint());
    }

    template<typename T>
    uint32_t Connection<T>::id() const {
        return id_;
    }

    template<typename T>
    void Connection<T>::send_message(Message<T> msg) {
        asio::post(socket_.get_executor(), [this, msg]() {
            bool writing = !out_queue_.empty();
            out_queue_.push(msg);

            if (!writing) {
                send_message_();
            }
        });
    }

    template<typename T>
    void Connection<T>::send_message_(Message<T> msg) {
        auto to_send = const_buffer_sequence(
            asio::buffer(&out_queue_.front().header, sizeof(typename Message<T>::Header)),
            asio::buffer(out_queue_.front().body.data(), out_queue_.front().body.size()));

        asio::async_write(socket_, to_send, [this](std::error_code ec, size_t length) {
            if (ec) {
                EZN_INFO("{:06d}: Failed to send a message' {}", this->id_, ec.message());
                close();
                return;
            }

            out_queue_.pop();
            if (out_queue_.empty()) {
                send_message_();
            }
        });
    }

    template<typename T>
    void Connection<T>::recv_message_() {
        asio::async_read(socket_, asio::buffer(&incoming_message_.header, sizeof(typename Message<T>::Header)),
                         [this](std::error_code ec, size_t length) {
                             if (ec) {
                                 EZN_INFO("{:06d}: Failed to read a header; {}", this->id_, ec.message());
                                 close();
                                 return;
                             }

                             if (incoming_message_.header.size > 0) {
                                 incoming_message_.body.resize(incoming_message_.header.size);
                                 asio::async_read(socket_, asio::buffer(incoming_message_.body.data(), incoming_message_.body.size()),
                                                  [this](std::error_code ec, size_t length) {
                                                      if (ec) {
                                                          EZN_INFO("{:06d}: Failed to read a body; {}", this->id_, ec.message());
                                                          close();
                                                          return;
                                                      }

                                                      on_message_receive_(incoming_message_, this->shared_from_this());
                                                      recv_message_();
                                                  });
                             } else {
                                 on_message_receive_(incoming_message_, this->shared_from_this());
                                 recv_message_();
                             }
                         });
    }
}  // namespace easy_net

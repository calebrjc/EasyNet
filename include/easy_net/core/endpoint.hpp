// File: easy_net/core/endpoint.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <iostream>
#include <string>

#include "easy_net/core/asio_stuff.hpp"

using asio::ip::tcp;

namespace easy_net {
    struct Endpoint {
        std::string address;
        std::string port;

        static Endpoint from_asio_endpoint(tcp::endpoint endpoint);

        friend std::ostream &operator<<(std::ostream &stream, const Endpoint &endpoint);
    };
}  // namespace easy_net

#include "easy_net/core/impl/endpoint.ipp"

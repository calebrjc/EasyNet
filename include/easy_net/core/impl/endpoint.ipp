// File: easy_net/core/impl/endpoint.ipp
// Author: Caleb Johnson-Cantrell

#pragma once

#include "easy_net/core/endpoint.hpp"

namespace easy_net {
    Endpoint Endpoint::from_asio_endpoint(tcp::endpoint endpoint) {
        return {endpoint.address().to_string(), std::to_string(endpoint.port())};
    }

    std::ostream &operator<<(std::ostream &stream, const Endpoint &endpoint) {
        stream << endpoint.address << ":" << endpoint.port;
        return stream;
    }
}  // namespace easy_net

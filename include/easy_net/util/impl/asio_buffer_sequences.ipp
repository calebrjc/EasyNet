// File: easy_net/util/impl/asio_buffer_sequences.ipp
// Author: Caleb Johnson-Cantrell

#pragma once

#include "easy_net/util/asio_buffer_sequences.hpp"

namespace easy_net {
    template<typename... Args>
    auto const_buffer_sequence(Args&&... args) {
        return std::array<asio::const_buffer, sizeof...(Args)> {asio::buffer(args)...};
    }

    template<typename... Args>
    auto buffer_sequence(Args&&... args) {
        return std::array<asio::mutable_buffer, sizeof...(Args)> {asio::buffer(args)...};
    }
}  // namespace easy_net

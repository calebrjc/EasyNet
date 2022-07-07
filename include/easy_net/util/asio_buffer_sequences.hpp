// File: easy_net/util/asio_buffer_sequences.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include "easy_net/core/asio_stuff.hpp"

namespace easy_net {
    template<typename... Args>
    auto const_buffer_sequence(Args&&... args);

    template<typename... Args>
    auto buffer_sequence(Args&&... args);
}  // namespace easy_net

#include "easy_net/util/impl/asio_buffer_sequences.ipp"

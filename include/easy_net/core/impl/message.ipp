// File: easy_net/core/impl/message.ipp
// Author: Caleb Johnson-Cantrell

#pragma once

#include "easy_net/core/message.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace easy_net {
    template<typename T>
    std::ostream &operator<<(std::ostream &stream, const Message<T> &message) {
        stream << "Message(type=" << int(message.header.type) << ", id=" << message.header.id
               << ", size=" << message.header.size << ")";
        return stream;
    }

    template<typename T, typename U>
    Message<T> &operator<<(Message<T> message, const U &data) {
        static_assert(std::is_standard_layout<U>::value, "The data is too complex to be held by this message");

        size_t original_size = message.body.size();
        message.body.resize(original_size + sizeof(U));
        std::memcpy(message.body.data() + original_size, &data, sizeof(U));
        message.header.size = (uint32_t)message.body.size();

        return message;
    }

    template<typename T, typename U>
    Message<T> &operator>>(Message<T> message, const U &data) {
        static_assert(std::is_standard_layout<U>::value, "The data is too complex to be pulled from this message");

        size_t new_size = message.body.size() - sizeof(U);
        std::memcpy(&data, message.body.data() + new_size, sizeof(U));
        message.body.resize(new_size);
        message.header.size = (uint32_t)message.body.size();

        return message;
    }
}  // namespace easy_net

// File: easy_net/core/impl/msg.ipp
// Author: Caleb Johnson-Cantrell

#pragma once

#include "easy_net/core/message.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace easy_net {
    template<typename U>
    std::ostream &operator<<(std::ostream &stream, const Message<U> &msg) {
        stream << "Message(type=" << int(msg.header.type) << ", id=" << msg.header.id
               << ", size=" << msg.header.size << ")";
        return stream;
    }

    template<typename U, typename V>
    Message<U> &operator<<(Message<U> &msg, const V &data) {
        static_assert(std::is_standard_layout<V>::value, "The data is too complex to be held by this msg");

        size_t original_size = msg.body.size();
        msg.body.resize(original_size + sizeof(V));
        std::memcpy(msg.body.data() + original_size, &data, sizeof(V));
        msg.header.size = (uint32_t)msg.body.size();

        return msg;
    }

    template<typename U, typename V>
    Message<U> &operator>>(Message<U> &msg, V &data) {
        static_assert(std::is_standard_layout<V>::value, "The data is too complex to be pulled from this msg");

        size_t new_size = msg.body.size() - sizeof(V);
        std::memcpy(&data, msg.body.data() + new_size, sizeof(V));
        msg.body.resize(new_size);
        msg.header.size = (uint32_t)msg.body.size();

        return msg;
    }
}  // namespace easy_net

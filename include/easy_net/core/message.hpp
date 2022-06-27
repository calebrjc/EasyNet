// File: easy_net/core/message.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <iostream>
#include <vector>

namespace simple_net {
    // Templated header type that allows users to create their own message types
    template<typename T>
    struct MessageHeader {
        // Identifies the type of the message that this header is attached to
        T type;

        // Identifies this message to separate it from others.
        uint32_t id = 0;

        // The size of the message that this header is attached to
        uint32_t size = 0;
    };

    // Templated message type encapsulating a message's data
    template<typename T>
    struct Message {
        // The header of this message
        MessageHeader<T> header;

        // The data that this message contains
        std::vector<uint8_t> body;

        // Inserts a nicely formatted string version of this message to the inputed `stream`
        template<typename U>
        friend std::ostream &operator<<(std::ostream &stream, const Message<U> &message);

        // Pushes data into this message
        // Ex: msg << x << y << z;
        //
        // Attribution: OneLoneCoder's PixelGameEngine networking extension.
        template<typename U>
        friend Message<T> &operator<<(Message<T> message, const U &data);

        // Pulls data from this message
        // Ex: msg >> z >> y >> x;
        //
        // Attribution: OneLoneCoder's PixelGameEngine networking extension.
        template<typename U>
        friend Message<T> &operator>>(Message<T> message, const U &data);
    };
}  // namespace simple_net

#include "easy_net/core/impl/message.ipp"

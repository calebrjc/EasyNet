// easy_net/asio_stuff.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#ifdef _WIN32
// Saves us from having to read some warnings
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

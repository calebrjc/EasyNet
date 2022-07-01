// File: easy_net/util/logging.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <memory>

#include <spdlog/spdlog.h>

#include <spdlog/fmt/ostr.h>

#ifdef NDEBUG
#define EZN_TRACE(...)
#define EZN_INFO(...)
#define EZN_WARN(...)
#define EZN_ERROR(...)
#define EZN_CRITICAL(...)
#else
#define EZN_TRACE(...)    spdlog::trace(__VA_ARGS__)
#define EZN_INFO(...)     spdlog::info(__VA_ARGS__)
#define EZN_WARN(...)     spdlog::warn(__VA_ARGS__)
#define EZN_ERROR(...)    spdlog::error(__VA_ARGS__)
#define EZN_CRITICAL(...) spdlog::critical(__VA_ARGS__)
#endif

// File: easy_net/core/impl/tsqueue.ipp
// Author: Caleb Johnson-Cantrell

#pragma once

#include "easy_net/core/tsqueue.hpp"

namespace easy_net {
    template<typename T>
    TSQueue<T>::TSQueue() = default;

    template<typename T>
    TSQueue<T>::~TSQueue() { clear(); }

    template<typename T>
    T &TSQueue<T>::front() {
        std::scoped_lock lock(deque_mux_);
        return deque_.front();
    }

    template<typename T>
    const T &TSQueue<T>::front() const {
        std::scoped_lock lock(deque_mux_);
        return deque_.front();
    }

    template<typename T>
    T &TSQueue<T>::back() {
        std::scoped_lock lock(deque_mux_);
        return deque_.back();
    }

    template<typename T>
    const T &TSQueue<T>::back() const {
        std::scoped_lock lock(deque_mux_);
        return deque_.back();
    }

    template<typename T>
    void TSQueue<T>::push(const T &object) {
        std::scoped_lock lock(deque_mux_);
        deque_.emplace_back(std::move(object));

        std::unique_lock<std::mutex> cv_lock(cv_mux_);
        cv_.notify_one();
    }

    template<typename T>
    T TSQueue<T>::pop() {
        std::scoped_lock lock(deque_mux_);

        auto object = std::move(deque_.front());
        deque_.pop_front();
        return object;
    }

    template<typename T>
    bool TSQueue<T>::is_empty() {
        std::scoped_lock lock(deque_mux_);
        return deque_.empty();
    }

    template<typename T>
    size_t TSQueue<T>::size() {
        std::scoped_lock lock(deque_mux_);
        return deque_.size();
    }

    template<typename T>
    void TSQueue<T>::clear() {
        std::scoped_lock lock(deque_mux_);
        deque_.clear();
    }

    template<typename T>
    void TSQueue<T>::wait() {
        while (is_empty()) {
            std::unique_lock cv_lock(cv_mux_);
            cv_.wait(cv_lock);
        }
    }
}  // namespace easy_net

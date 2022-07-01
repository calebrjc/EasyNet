// File: easy_net/core/tsqueue.hpp
// Author: Caleb Johnson-Cantrell

#pragma once

#include <cstdlib>

#include <condition_variable>
#include <mutex>
#include <queue>

namespace easy_net {
    // A thread-safe (locked) implementation of a generic queue data structure
    template<typename T>
    class TSQueue {
      public:
        // Constructs a queue
        TSQueue();

        // A queue cannot be copy constructed
        TSQueue(const TSQueue<T> &other) = delete;

        // Destructs this queue
        virtual ~TSQueue();

        // Returns a reference to the object at the front of this queue
        T &front();
        const T &front() const;

        // Returns a reference to the object at the back of this queue
        T &back();
        const T &back() const;

        // Adds an object to the back of this queue
        void push(const T &object);

        // Returns the object at the front of this queue (after removing it)
        T pop();

        // Returns true if this queue contains no elements
        bool is_empty();

        // Returns the number of object in this queue
        size_t size();

        // Empties this queue
        void clear();

        // Suspends the calling thread until this queue is not empty
        void wait();

      private:
        std::queue<T> queue_;
        std::mutex queue_mux_;

        std::condition_variable cv_;
        std::mutex cv_mux_;
    };
}  // namespace easy_net

#include "easy_net/core/impl/tsqueue.ipp"

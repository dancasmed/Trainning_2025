#ifndef LOCK_FREE_QUEUE
#define LOCK_FREE_QUEUE

#include <queue>
#include <mutex>
#include "MarketData.h"

/**
 * @class ThreadSafeQueue
 * @brief A thread-safe queue for MarketData objects.
 *
 * This class provides a thread-safe implementation of a queue that can be used in multi-threaded environments.
 * It uses a mutex to ensure that only one thread can access the queue at any given time, thus preventing race conditions.
 */
class ThreadSafeQueue {
    private:
        std::queue<MarketData> queue; ///< The underlying queue storage for MarketData objects
        std::mutex mtx;              ///< Mutex used to synchronize access to the queue

    public:
        /**
         * @brief Adds a new element to the end of the queue.
         *
         * This function is thread-safe and can be called concurrently from multiple threads.
         * The mutex ensures that only one thread can modify the queue at any given time.
         *
         * @param data A constant reference to the MarketData object to be added to the queue
         */
        void push(const MarketData& data);

        /**
         * @brief Removes the front element from the queue and copies it into the provided reference.
         *
         * This function is thread-safe and can be called concurrently from multiple threads.
         * The mutex ensures that only one thread can access the queue at any given time.
         *
         * If the queue is empty when this function is called, the provided MarketData reference will remain unchanged,
         * and the function will return false.
         *
         * @param data A reference to a MarketData object where the front element of the queue will be copied if successful
         * @return True if an element was successfully removed and copied, false otherwise (i.e., if the queue is empty)
         */
        bool pop(MarketData& data);
};

#endif

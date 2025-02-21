#include "MarketData.h"
#include "ThreadSafeQueue.h"

/**
 * @brief Thread-safe push operation to add market data to the queue
 *
 * This function safely adds a MarketData object to the end of the queue.
 * The mutex ensures that only one thread can modify the queue at any time.
 *
 * @param data A constant reference to the MarketData object to be added
 */
void ThreadSafeQueue::push(const MarketData& data) {
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(data);
}

/**
 * @brief Thread-safe pop operation to remove market data from the queue
 *
 * This function attempts to safely remove the front element of the queue and
 * assigns it to the provided MarketData reference. If the queue is empty, it returns false.
 * The mutex ensures that only one thread can access the queue at any time.
 *
 * @param data A reference to the MarketData object where the result will be stored
 * @return true if an element was successfully removed and assigned, false if the queue is empty
 */
bool ThreadSafeQueue::pop(MarketData& data) {
    std::lock_guard<std::mutex> lock(mtx);

    if (queue.empty()) {
        return false;
    }

    data = queue.front();
    queue.pop();
    return true;
}

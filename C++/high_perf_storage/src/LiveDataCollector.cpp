#include <iostream>
#include "MarketData.h"
#include "ThreadSafeQueue.h"
#include "LiveDataCollector.h"

/**
 * @brief Construct a LiveDataCollector instance.
 *
 * Initializes the LiveDataCollector with a BufferedWriter to handle data writing operations.
 *
 * @param bufferedWriter Pointer to the BufferedWriter used for writing collected market data.
 */
LiveDataCollector::LiveDataCollector(BufferedWriter* bufferedWriter)
    : _bufferedWriter(bufferedWriter) {
}

/**
 * @brief Collects and processes live market data from a thread-safe queue.
 *
 * This method continuously collects MarketData objects from the provided ThreadSafeQueue
 * while the runningFlag is set to true. For each successfully retrieved MarketData object,
 * it prints the data to the console and writes it to the buffered writer.
 *
 * @param queue The thread-safe queue containing MarketData objects to be processed.
 * @param runningFlag Atomic flag controlling the loop execution (true = continue, false = exit).
 * @param id Unique identifier for the LiveDataCollector instance.
 */
void LiveDataCollector::collect(ThreadSafeQueue& queue, std::atomic<bool>& runningFlag, int id) {
    while (runningFlag) {
        MarketData data;
        if (queue.pop(data)) {
            auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(data.timestamp.time_since_epoch()).count();
            std::cout << "LiveDataCollector - Id: " << id << " | Time: " << milliseconds_since_epoch << " | Price: " << data.price << " | Volume: " << data.volume <<
std::endl;
            _bufferedWriter->write(data);
        }
    }
}

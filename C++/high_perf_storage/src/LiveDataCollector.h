#ifndef DATA_CONSUMER
#define DATA_CONSUMER

#include <atomic>
#include "ThreadSafeQueue.h"
#include "BufferedWriter.h"

/**
 * @class LiveDataCollector
 * @brief A class responsible for collecting live data from a thread-safe queue and writing it to an output destination.
 *
 * This class provides functionality to continuously collect data from a ThreadSafeQueue and write it using a BufferedWriter.
 * The collection process runs in a loop until a provided running flag is set to false.
 */
class LiveDataCollector {
private:
    BufferedWriter* _bufferedWriter; ///< Pointer to the buffered writer used for writing collected data to the output

public:
    /**
     * @brief Constructor for LiveDataCollector
     * @param bufferedWriter Pointer to the BufferedWriter instance to be used for writing data
     *
     * Initializes the LiveDataCollector with the provided BufferedWriter. The provided writer must not be null.
     */
    LiveDataCollector(BufferedWriter* bufferedWriter);

    /**
     * @brief Collects live data from the provided queue and writes it using the buffered writer
     * @param queue The thread-safe queue containing the data to be collected
     * @param runningFlag Atomic flag indicating whether the collection process should continue
     * @param id Identifier for the collector instance (useful for logging or identification)
     *
     * This function continuously collects data from the provided ThreadSafeQueue and writes it using the buffered writer.
     * The loop runs until the runningFlag is set to false. The function may block if the queue is empty.
     *
     * @exception std::exception Thrown if an error occurs during the write operation
     */
    void collect(ThreadSafeQueue& queue, std::atomic<bool> &runningFlag, int id);
};

#endif
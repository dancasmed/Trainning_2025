#include <iostream>
#include <fstream>
#include <mutex>
#include <ctime>
#include <string>
#include <thread>
#include <zstd.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "BufferedWriter.h"
#include "MarketData.h"

/**
 * @brief Constructor for BufferedWriter
 *
 * Initializes a new instance of the BufferedWriter class with the specified filename prefix,
 * buffer size, and DataWriter instance. This constructor sets up the initial state required
 * for buffered writing operations.
 *
 * @param[in] filenamePrefix The prefix used when creating output filenames
 * @param[in] bufferSize The maximum number of elements the buffer can hold before flushing
 * @param[in] dataWriter Pointer to the DataWriter instance for persisting data. Note: This class does not take ownership of the DataWriter instance.
 */
BufferedWriter :: BufferedWriter(const std::string& filenamePrefix, unsigned long bufferSize, DataWriter* dataWriter)
: _filenamePrefix(filenamePrefix), _size(bufferSize), _dataWriter(dataWriter) {
}

/**
 * @brief Destructor for BufferedWriter
 *
 * Ensures that any remaining buffered data is flushed to disk and the writer thread is properly joined.
 * This destructor guarantees that no data remains in the buffer and all resources are released before
 * the object is destroyed. If the buffer contains unwritten data when this destructor is called,
 * it will be automatically flushed.
 */
BufferedWriter :: ~BufferedWriter() {
    if (_buffer.size() > 0) {
        flush(); // Flush remaining data
    }
    if (_writerThread != NULL && _writerThread->joinable()) {
        _writerThread->join(); // Ensure writer thread is completed
    }
}

/**
 * @brief Adds MarketData to buffer and flushes when buffer limit is reached
 *
 * This method appends the provided MarketData object to the internal buffer. If the buffer reaches its
 * capacity, it triggers a flush operation to persist the data to disk. The method ensures thread-safe
 * access to the buffer using a mutex.
 *
 * @param[in] data The MarketData object to be added to the buffer
 */
void BufferedWriter :: write(const MarketData& data) {
    std::lock_guard<std::mutex> lock(mtx);  // Thread-safe access to buffer
    _buffer.emplace_back(data);  // Add data to buffer
    if (_buffer.size() >= _size) {
        std::cout << "Saving data ..." << std::endl;
        flush();  // Flush data if buffer is full or exactly at capacity
    }
}

/**
 * @brief Flushes buffer by delegating data writing to a separate thread
 *
 * This method checks if there is buffered data and initiates the flushing process. It ensures that any
 * ongoing write operations are completed before starting a new one, preventing race conditions.
 * The actual writing operation is delegated to a background thread for asynchronous processing.
 *
 * If a previous flush operation is still in progress, this method will wait for it to complete
 * before starting a new one. A warning message is printed in such cases to indicate the delay.
 */
void BufferedWriter :: flush() {
    if (_buffer.size() > 0) {
        // Wait for any ongoing flush operations to complete
        if (_writerThread != NULL && _writerThread->joinable()) {
            std::cout << "Warning: Waiting for Data Writer thread to finish. To avoid this increase the buffer size." << std::endl;
            _writerThread->join();  // Synchronize with previous thread
        }

        // Start a new asynchronous write operation
        // Using move semantics to transfer ownership of buffer contents
        _writerThread = std::make_unique<std::thread>(std::thread(&DataWriter::writeData, _dataWriter, std::move(_buffer)));
    }
}

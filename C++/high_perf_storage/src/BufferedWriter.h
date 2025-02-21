#ifndef BUFFERED_WRITER
#define BUFFERED_WRITER

#include <mutex>
#include <thread>
#include "MarketData.h"
#include "DataWriter.h"

/**
 * @brief Provides buffered and asynchronous writing of MarketData objects
 *
 * The BufferedWriter class is responsible for buffering MarketData objects and writing them
 * asynchronously to a file. It ensures thread-safe access to the buffer and performs automatic
 * flushing when the buffer reaches its capacity. This class is particularly useful in scenarios
 * where data persistence needs to be efficient and non-blocking.
 */
class BufferedWriter {
private:
    std::vector<MarketData> _buffer;  ///< Temporary storage for MarketData objects before they are written to disk.
    std::mutex mtx;                   ///< Mutex to ensure thread-safe access to the buffer and other critical sections.
    const std::string _filenamePrefix; ///< Prefix used by the DataWriter to generate filenames for output files.
    const unsigned long _size;        ///< Maximum number of elements the buffer can hold before an automatic flush is triggered.
    DataWriter* _dataWriter;          ///< Pointer to a DataWriter instance responsible for persisting data to disk.
    std::unique_ptr<std::thread> _writerThread;  ///< Manages the thread responsible for asynchronous writing operations.

public:
    /**
     * @brief Flushes the buffer by writing its contents to disk
     *
     * This method ensures that all data in the buffer is written to disk immediately. It can be called
     * manually if an immediate flush is required, or it will be triggered automatically when the buffer
     * reaches its capacity.
     */
    void flush();

    /**
     * @brief Constructor for BufferedWriter
     *
     * Initializes a new instance of the BufferedWriter class with the specified filename prefix,
     * buffer size, and DataWriter instance. The constructor does not throw exceptions but ensures that
     * all resources are properly initialized.
     *
     * @param filenamePrefix The prefix used when creating output filenames
     * @param bufferSize The maximum number of elements the buffer can hold before flushing
     * @param dataWriter Pointer to the DataWriter instance for persisting data
     */
    BufferedWriter(const std::string& filenamePrefix, unsigned long bufferSize, DataWriter* dataWriter);

    /**
     * @brief Destructor for BufferedWriter
     *
     * Ensures that any remaining buffered data is flushed to disk before the object is destroyed.
     * The destructor waits for any ongoing flush operations to complete to ensure data integrity.
     */
    ~BufferedWriter();

    /**
     * @brief Adds MarketData to the buffer and flushes if buffer is full
     *
     * Appends the specified MarketData object to the buffer. If adding this object causes the buffer to
     * exceed its capacity, the method triggers an automatic flush of the buffer contents to disk.
     * This method is thread-safe and ensures that data is handled correctly in multi-threaded environments.
     *
     * @param data The MarketData object to be added to the buffer
     */
    void write(const MarketData& data);
};

#endif  // BUFFERED_WRITER

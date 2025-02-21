#ifndef DATA_WRITER
#define DATA_WRITER

#include <iostream>
#include "MarketData.h"

/**
 * @brief Class responsible for writing market data to files with optional compression and redundancy.
 *
 * This class handles the serialization and storage of market data. It supports multiple output directories,
 * filename prefixing, data compression, and configurable redundancy levels.
 */
class DataWriter {
    private:
        const std::vector<std::string> _outputDirectories; ///< Vector of output directory paths where the data will be written.
        const std::string _filenamePrefix = "data_"; ///< Prefix for generated filenames.
        const bool _compressionEnabled; ///< Flag indicating whether data compression is enabled.
        int _redundancyLevel; ///< Level of redundancy for data storage (number of copies).

        /**
         * @brief Opens a file and returns its file descriptor.
         *
         * Attempts to open the specified file for writing. Returns the file descriptor if successful, -1 otherwise.
         *
         * @param fileName The name of the file to open
         * @return File descriptor (int) on success, -1 on failure
         */
        int openFile(const std::string& fileName);

        /**
         * @brief Compresses market data into a buffer.
         *
         * Compresses the provided market data and stores it in the given buffer. Updates the size of the compressed data.
         *
         * @param data The market data to compress
         * @param buffer The buffer to store the compressed data
         * @param dataSize Reference to a size_t where the size of the compressed data will be stored
         * @return true if compression was successful, false otherwise
         */
        bool compressDataIntoBuffer(const std::vector<MarketData>& data, std::vector<char>& buffer, size_t& dataSize);

        /**
         * @brief Copies market data into a buffer without compression.
         *
         * Copies the raw market data directly into the provided buffer. Updates the size of the copied data.
         *
         * @param data The market data to copy
         * @param buffer The buffer to store the raw data
         * @param dataSize Reference to a size_t where the size of the raw data will be stored
         */
        void copyDataToBuffer(const std::vector<MarketData>& data, std::vector<char>& buffer, size_t& dataSize);

        /**
         * @brief Maps and writes buffered data to a file.
         *
         * Writes the contents of the provided buffer to the specified file descriptor. Handles memory mapping for efficient I/O operations.
         *
         * @param fd The file descriptor to write to
         * @param buffer The buffer containing the data to write
         * @param dataSize The size of the data in the buffer
         * @return true if writing was successful, false otherwise
         */
        bool mapAndWriteToFile(int fd, std::vector<char>& buffer, size_t dataSize);

        /**
         * @brief Generates a filename for the output file.
         *
         * This function creates a filename by combining the filename prefix with the current
         * datetime in "YYYY-MM-DD_HH:MM:SS" format and appends ".bin" extension.
         *
         * @return A generated filename string
         */
        std::string getFileName();

        /**
         * @brief Creates a directory if it does not already exist.
         *
         * Ensures that the specified directory exists on the filesystem, creating it if necessary.
         *
         * @param path The directory path to create
         */
        void createDirectoryIfNotExists(const std::string& path);

        /**
         * @brief Creates multiple directories if they do not already exist.
         *
         * Ensures that all specified directories exist on the filesystem, creating them as necessary.
         *
         * @param directories A vector of directory paths to create
         */
        void createDirectories(const std::vector<std::string>& directories);

        /**
         * @brief Generates a list of redundant directories for data storage.
         *
         * Creates a list of directory paths based on the configured redundancy level and output directories.
         *
         * @return A vector of directory strings where redundant copies will be stored
         */
        std::vector<std::string> getRedundantDirectories();

        /**
         * @brief Writes market data to a file with optional compression and redundancy.
         *
         * Handles the complete process of writing market data to one or more files, including directory creation,
         * filename generation, and data serialization with optional compression.
         *
         * @param data The market data to write
         * @param fileName The base name for the output file(s)
         */
        void writeToFile(const std::vector<MarketData>& data, std::string fileName);

    public:
        /**
         * @brief Constructor for DataWriter.
         *
         * Initializes a DataWriter instance with the specified compression settings, redundancy level,
         * and output directories.
         *
         * @param compressionEnabled Whether to enable data compression
         * @param redundancyLevel Number of redundant copies to maintain
         * @param outputDirectories Vector of directory paths where data will be written
         */
        DataWriter(bool compressionEnabled, int redundancyLevel, std::vector<std::string> outputDirectories);

        /**
         * @brief Writes market data to files.
         *
         * This method handles all aspects of writing the provided market data to one or more files,
         * including directory creation, filename generation, and data serialization with optional
         * compression based on the class configuration.
         *
         * @param data The vector of MarketData objects to write to file(s)
         */
        void writeData(const std::vector<MarketData> &data);
};

#endif

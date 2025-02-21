#ifndef HISTORICAL_DATA_READER
#define HISTORICAL_DATA_READER

#include <unordered_map>
#include "MarketData.h"

/**
 * @class HistoricalDataReader
 * @brief A class responsible for reading and parsing historical market data from files.
 *
 * This class provides functionality to search for data files in specified directories,
 * load the data, and optionally decompress it before parsing into MarketData objects.
 */
class HistoricalDataReader {
    private:
        const bool _compressionEnabled; ///< Flag indicating whether data compression is enabled.
        std::vector<std::string> _dataDirectories; ///< List of directories to search for data files.

        /**
         * @brief Searches recursively through a directory and its subdirectories for data files.
         *
         * This method collects all unique data file paths and stores them in the provided map.
         *
         * @param directory The directory path to start the search from.
         * @param uniqueFiles A reference to an unordered_map that will store the unique file paths.
         */
        void searchDataFiles(std::string directory, std::unordered_map<std::string, std::string>& uniqueFiles);

        /**
         * @brief Loads market data from a specified file.
         *
         * This method reads the contents of the file, decompresses it if necessary, and parses the data into MarketData objects.
         *
         * @param fileName The name of the file to load data from.
         * @return A vector of MarketData objects containing the parsed data from the file.
         */
        std::vector<MarketData> loadFromFile(std::string fileName);

        /**
         * @brief Opens a file and returns its file descriptor.
         *
         * This method opens the specified file in read-only mode and returns the associated file descriptor.
         *
         * @param fileName The name of the file to open.
         * @return The file descriptor for the opened file.
         */
        int openFile(const std::string& fileName);

        /**
         * @brief Retrieves the size of an open file.
         *
         * This method uses the file descriptor to determine the size of the file in bytes.
         *
         * @param fd The file descriptor of the open file.
         * @return The size of the file in bytes.
         */
        size_t getFileSize(int fd);

        /**
         * @brief Loads the contents of a file into a buffer.
         *
         * This method reads all data from the specified file and stores it in a buffer for further processing.
         *
         * @param fd The file descriptor of the open file.
         * @param dataSize The size of the data to read.
         * @return A vector containing the raw data bytes loaded from the file.
         */
        std::vector<char> loadFileToBuffer(int fd, size_t dataSize);

        /**
         * @brief Decompresses and parses raw data into MarketData objects.
         *
         * This method takes a buffer of raw data, decompresses it if necessary, and then parses it into a vector of MarketData objects.
         *
         * @param buffer The buffer containing the raw data to process.
         * @return A vector of MarketData objects representing the parsed data.
         */
        std::vector<MarketData> decompressAndParseData(const std::vector<char>& buffer);

        /**
         * @brief Parses raw data into MarketData objects without decompression.
         *
         * This method is used when compression is disabled, taking a buffer of raw data and directly parsing it into MarketData objects.
         *
         * @param buffer The buffer containing the raw data to process.
         * @param dataSize The size of the data in bytes.
         * @return A vector of MarketData objects representing the parsed data.
         */
        std::vector<MarketData> parseData(const std::vector<char>& buffer, size_t dataSize);

    public:
        /**
         * @brief Constructor for HistoricalDataReader.
         *
         * Initializes the object with the specified compression flag and directories to search for data files.
         *
         * @param compressionEnabled A boolean indicating whether data decompression should be enabled.
         * @param dataDirectories A vector of strings representing the directories where data files are located.
         */
        HistoricalDataReader(const bool compressionEnabled, const std::vector<std::string>& dataDirectories);

        /**
         * @brief Reads all historical market data from the specified directories.
         *
         * This method searches for all available data files in the specified directories, loads each file,
         * and returns a consolidated vector of MarketData objects containing all the parsed data.
         *
         * @return A vector of MarketData objects representing all the historical market data found.
         */
        std::vector<MarketData> readAll();
};

#endif

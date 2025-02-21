#include <fstream>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <fcntl.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zstd.h>


#include "HistoricalDataReader.h"

/**
 * @brief Constructor for HistoricalDataReader
 *
 * Initializes the reader with compression enabled flag and data directories
 *
 * @param[in] compressionEnabled Flag indicating whether compression is enabled
 * @param[in] dataDirectories Vector of paths to directories containing market data files
 */
HistoricalDataReader::HistoricalDataReader(const bool compressionEnabled, const std::vector<std::string>& dataDirectories)
    : _compressionEnabled(compressionEnabled), _dataDirectories(dataDirectories) {
}

/**
 * @brief Reads all available historical market data from specified directories
 *
 * This function scans through all specified directories for valid market data files,
 * loads their contents, and combines them into a single dataset.
 *
 * @return A vector of MarketData objects containing all loaded records
 */
std::vector<MarketData> HistoricalDataReader::readAll() {
    std::vector<MarketData> loadedData;
    std::unordered_map<std::string, std::string> uniqueFiles;

    // Iterate through each data directory to find market data files
    for (auto& directory : _dataDirectories) {
        searchDataFiles(directory, uniqueFiles);
    }

    // Load data from each identified file and combine into the result set
    for (const auto& pair : uniqueFiles) {
        std::cout << "FileToRead: " << pair.second << std::endl;
        std::vector<MarketData> fileData = loadFromFile(pair.second);
        loadedData.insert(loadedData.begin(), fileData.begin(), fileData.end());
    }

    // Output the total number of records loaded
    std::cout << "Loaded data records: " << loadedData.size() << std::endl;

    return loadedData;
}

/**
 * @brief Scans a directory for market data files and populates a map of unique files
 *
 * This function searches through a given directory to find files matching the naming pattern,
 * then stores their relative paths in a map to ensure uniqueness.
 *
 * @param[in] directory Path to the directory to search
 * @param[out] uniqueFiles Map from filenames to relative file paths (will be populated)
 */
void HistoricalDataReader::searchDataFiles(std::string directory, std::unordered_map<std::string, std::string>& uniqueFiles) {
    try {
        // Iterate through all entries in the directory
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string fileName = entry.path().filename().string();

                // Check if file matches the expected naming convention: "data_*.bin"
                if (fileName.starts_with("data_") && fileName.ends_with(".bin")) {
                    std::cout << "uniqueFiles: " << fileName << " -> " << entry.path().relative_path() << std::endl;
                    uniqueFiles[fileName] = entry.path().relative_path();
                }
            }
        }

    } catch (const std::filesystem::directory_entry& e) {
        // Handle directory-related exceptions
        std::cerr << "Directory entry exception: " << e << std::endl;
    } catch (const std::exception& e) {
        // Catch-all for other potential exceptions
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

/**
 * @brief Loads historical market data from a specified file.
 *
 * This function reads market data from a file, decompresses it if necessary,
 * and parses it into a vector of MarketData objects. The file can be in a
 * compressed or uncompressed format depending on the _compressionEnabled flag.
 *
 * @param fileName The name of the file to load data from.
 * @return std::vector<MarketData> A vector containing the loaded market data.
 */
std::vector<MarketData> HistoricalDataReader::loadFromFile(std::string fileName) {
    std::vector<MarketData> loadedData;
    std::string finalFileName = fileName;

    int fd = openFile(finalFileName);
    if (fd == -1) return loadedData;

    size_t dataSize = getFileSize(fd);
    if (dataSize == 0) {
        close(fd);
        return loadedData;
    }

    std::vector<char> buffer = loadFileToBuffer(fd, dataSize);
    close(fd);

    if (_compressionEnabled) {
        loadedData = decompressAndParseData(buffer);
    } else {
        loadedData = parseData(buffer, dataSize);
    }

    return loadedData;
}

/**
 * @brief Opens a file for reading.
 *
 * Attempts to open the specified file in read-only mode. On failure,
 * an error message is printed to cerr.
 *
 * @param fileName The name of the file to open.
 * @return int The file descriptor if successful; -1 on failure.
 */
int HistoricalDataReader::openFile(const std::string& fileName) {
    std::cout << "Opening file ..." << std::endl;
    int fd = open(fileName.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening the file " << fileName << std::endl;
    }
    return fd;
}

/**
 * @brief Retrieves the size of an opened file.
 *
 * Uses fstat to get the size of the file associated with the given
 * file descriptor. On failure, prints an error message and returns 0.
 *
 * @param fd The file descriptor of the open file.
 * @return size_t The size of the file in bytes; 0 on failure.
 */
size_t HistoricalDataReader::getFileSize(int fd) {
    std::cout << "Getting file size ..." << std::endl;
    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
        std::cerr << "Error getting file size." << std::endl;
        return 0;
    }
    return fileStat.st_size;
}

/**
 * @brief Loads file data into a memory buffer using mmap.
 *
 * This function maps a file into memory, copies its contents into a buffer, and then unmaps it.
 *
 * @param fd File descriptor of the open file to load.
 * @param dataSize Size of the data to be loaded from the file.
 * @return std::vector<char> A vector containing the file data. If an error occurs during mmap, the returned buffer will be empty but initialized.
 */
std::vector<char> HistoricalDataReader::loadFileToBuffer(int fd, size_t dataSize) {
    std::cout << "Loading file into memory ..." << std::endl;
    std::vector<char> buffer(dataSize);

    void* map = mmap(nullptr, dataSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        std::cerr << "Error in mmap." << std::endl;
        return buffer;
    }

    std::cout << "Copying data into data buffer ..." << std::endl;
    memcpy(buffer.data(), map, dataSize);

    std::cout << "Closing file ..." << std::endl;
    if (munmap(map, dataSize) == -1) {
        std::cerr << "Error in munmap." << std::endl;
    }

    return buffer;
}

/**
 * @brief Decompresses and parses compressed data from a buffer.
 *
 * This function decompresses Zstandard-compressed data stored in the provided buffer,
 * then parses the decompressed data into MarketData objects.
 *
 * @param buffer The vector containing the compressed data to be decompressed and parsed.
 * @return std::vector<MarketData> A vector of MarketData objects. If an error occurs, returns an empty vector.
 */
std::vector<MarketData> HistoricalDataReader::decompressAndParseData(const std::vector<char>& buffer) {
    std::vector<MarketData> loadedData;

    std::cout << "Decompressing data in buffer ..." << std::endl;
    size_t decompressedSize = ZSTD_getFrameContentSize(buffer.data(), buffer.size());
    if (decompressedSize == ZSTD_CONTENTSIZE_ERROR || decompressedSize == ZSTD_CONTENTSIZE_UNKNOWN) {
        std::cerr << "Error determining decompressed size." << std::endl;
        return loadedData;
    }

    std::vector<char> decompressedBuffer(decompressedSize);
    size_t result = ZSTD_decompress(decompressedBuffer.data(), decompressedSize,
                                    buffer.data(), buffer.size());

    if (ZSTD_isError(result)) {
        std::cerr << "Decompression error: " << ZSTD_getErrorName(result) << std::endl;
        return loadedData;
    }

    std::cout << "Getting Market Data from buffer ..." << std::endl;
    size_t numElements = decompressedSize / sizeof(MarketData);
    loadedData.resize(numElements);
    memcpy(loadedData.data(), decompressedBuffer.data(), decompressedSize);

    return loadedData;
}

/**
 * @brief Parses raw data from a buffer into MarketData objects.
 *
 * This function assumes the provided buffer contains uncompressed data that can be directly
 * parsed into MarketData objects. It calculates the number of elements based on the buffer size,
 * resizes the output vector accordingly, and copies the data into it.
 *
 * @param buffer The vector containing raw data to be parsed.
 * @param dataSize The total size of the data in the buffer.
 * @return std::vector<MarketData> A vector of MarketData objects. If an error occurs during parsing, returns an empty vector.
 */
std::vector<MarketData> HistoricalDataReader::parseData(const std::vector<char>& buffer, size_t dataSize) {
    std::cout << "Getting Market Data from buffer ..." << std::endl;
    std::vector<MarketData> loadedData;

    size_t numElements = dataSize / sizeof(MarketData);
    loadedData.resize(numElements);
    memcpy(loadedData.data(), buffer.data(), dataSize);

    return loadedData;
}

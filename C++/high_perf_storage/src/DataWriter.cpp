#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <zstd.h>
#include <algorithm>
#include <random>
#include "DataWriter.h"

/**
 * @brief Constructor for DataWriter class
 *
 * Initializes the DataWriter with specified output directories, compression enabled flag,
 * and redundancy level. Creates the necessary output directories if they do not exist, in 
 * a real case this must not happen and existing directories should be mapped to remote 
 * directories in other hosts.
 *
 * @param[in] compressionEnabled Flag indicating whether data should be compressed
 * @param[in] redundancyLevel Level of redundancy for data writing
 * @param[in] outputDirectories Vector of output directory paths to write data to
 */
DataWriter :: DataWriter(bool compressionEnabled, int redundancyLevel, std::vector<std::string> outputDirectories)
: _outputDirectories(outputDirectories), _compressionEnabled(compressionEnabled), _redundancyLevel(redundancyLevel)
{
    createDirectories(_outputDirectories);
}

/**
 * @brief Writes market data to redundant directories
 *
 * Writes the provided market data to multiple redundant directories as specified by
 * the redundancy level. Each directory will contain an identical copy of the data.
 *
 * @param[in] data Vector of MarketData objects to be written
 */
void DataWriter :: writeData(const std::vector<MarketData> &data) {
    // Get list of redundant directories based on redundancy level
    std::vector<std::string> redundantDirectories = getRedundantDirectories();

    for (const auto& dir : redundantDirectories) {

        // Construct the full file path using directory and generated file name
        std::string finalFileName = dir + "/" + getFileName();

        // Write data to the constructed file path
        writeToFile(data, finalFileName);
    }
}

/**
 * @brief Writes market data to a specified file.
 *
 * This function writes the provided market data to a file. It supports both compressed and uncompressed writing based on the _compressionEnabled flag.
 * The function handles file operations including opening, writing, and closing the file.
 *
 * @param data A vector containing MarketData objects to be written to the file.
 * @param fileName The name of the file to write the data to.
 *
 * @return None
 */
void DataWriter :: writeToFile(const std::vector<MarketData> &data, std::string fileName)
{
    int fd = openFile(fileName);
    if (fd == -1) return;

    size_t dataSize;
    std::vector<char> buffer;

    // Prepare the buffer with either compressed or uncompressed data
    if (_compressionEnabled) {
        if (!compressDataIntoBuffer(data, buffer, dataSize)) {
            close(fd);
            return;
        }
    } else {
        copyDataToBuffer(data, buffer, dataSize);
    }

    // Map and write the prepared buffer to the file
    if (!mapAndWriteToFile(fd, buffer, dataSize)) {
        close(fd);
        return;
    }

    close(fd);
}

/**
 * @brief Opens a file for reading and writing.
 *
 * This function opens a file in read-write mode, creating it if it does not already exist. On failure, an error message is printed to cerr.
 *
 * @param fileName The name of the file to open.
 *
 * @return int A valid file descriptor on success; -1 on failure.
 */
int DataWriter :: openFile(const std::string& fileName) {
    int fd = open(fileName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        std::cerr << "Error opening file " << fileName << std::endl;
    }
    return fd;
}

/**
 * @brief Compresses market data into a buffer using Zstandard compression.
 *
 * This function takes a vector of MarketData objects, compresses the data,
 * and stores the result in the provided buffer. It also updates the dataSize
 * parameter to reflect the size of the compressed data.
 *
 * @param[in]     data      The vector of MarketData objects to be compressed.
 * @param[in,out] buffer    The buffer where the compressed data will be stored.
 * @param[in,out] dataSize The size of the uncompressed data, which will be updated
 *                        to the size of the compressed data upon successful completion.
 *
 * @return true if the compression was successful, false otherwise.
 */
bool DataWriter :: compressDataIntoBuffer(const std::vector<MarketData>& data,
    std::vector<char>& buffer,
    size_t& dataSize) {
        dataSize = data.size() * sizeof(MarketData);
        buffer.resize(ZSTD_compressBound(dataSize));

        size_t compressedSize = ZSTD_compress(buffer.data(), buffer.size(), data.data(), dataSize, 3);

    if (ZSTD_isError(compressedSize)) {
        std::cerr << "Error in compression process: " << ZSTD_getErrorName(compressedSize) << std::endl;
        return false;
    }

    dataSize = compressedSize;
    buffer.resize(dataSize); // Adjust the compressed buffer size
    return true;
}

/**
* @brief Copies market data directly into a buffer without compression.
*
* This function copies the raw data from the MarketData vector into the provided
* buffer. The dataSize parameter is updated to reflect the total size of the data.
*
* @param[in]     data      The vector of MarketData objects to be copied.
* @param[in,out] buffer    The buffer where the raw data will be stored.
* @param[in,out] dataSize The size of the data, which will be set to the total
*                        size of the MarketData objects upon completion.
*
* @return void
*/
void DataWriter :: copyDataToBuffer(const std::vector<MarketData>& data, std::vector<char>& buffer, size_t& dataSize) {
    dataSize = data.size() * sizeof(MarketData);
    buffer.resize(dataSize);
    memcpy(buffer.data(), data.data(), dataSize);
}

/**
* @brief Maps and writes the buffer contents to a file using memory mapping.
*
* This function maps the provided buffer to a file, ensuring that the file size
* is adjusted appropriately. It then copies the buffer's contents into the mapped
* memory space and ensures that changes are synchronized to disk before unmapping.
*
* @param[in] fd        The file descriptor of the file to be written to.
* @param[in] buffer    The buffer containing the data to be written.
* @param[in] dataSize The size of the data in the buffer.
*
* @return true if the operation was successful, false otherwise.
*/
bool DataWriter :: mapAndWriteToFile(int fd, std::vector<char>& buffer, size_t dataSize) {
    // Adjust the file size
    if (ftruncate(fd, dataSize) == -1) {
        std::cerr << "Error adjusting the file size." << std::endl;
    return false;
    }

    // Mapping file to memory
    void* map = mmap(nullptr, dataSize, PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        std::cerr << "Error while mapping file to memory." << std::endl;
    return false;
    }

    // Copy data into the mapped memory
    memcpy(map, buffer.data(), dataSize);

    // Ensure the data changes are written to the file
    if (msync(map, dataSize, MS_SYNC) == -1) {
        std::cerr << "Error while synchronizing the file in memory." << std::endl;
    }

    // Unmapping memory
    if (munmap(map, dataSize) == -1) {
        std::cerr << "Error while removing file from memory." << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Generates a filename with timestamp based on the current date and time
 *
 * This function creates a filename by combining the filename prefix with the current
 * datetime in "YYYY-MM-DD_HH:MM:SS" format and appends ".bin" extension.
 *
 * @return std::string The generated filename with timestamp
 */
std::string DataWriter :: getFileName() {
    // Get current time as a time_t object
    std::time_t time = std::time(nullptr);

    // Convert to local time structure
    std::tm* localTime = std::localtime(&time);

    char DateTime[20];
    // Format the datetime into a string: YYYY-MM-DD_HH:MM:SS
    std::strftime(DateTime, sizeof(DateTime), "%Y-%m-%d_%H:%M:%S", localTime);

    // Combine prefix, datetime and extension to create final filename
    return _filenamePrefix + DateTime + ".bin";
}

/**
 * @brief Creates a directory if it doesn't already exist
 *
 * This function checks if the specified directory exists. If it doesn't exist,
 * it attempts to create it.
 * If the path exists but isn't a directory, an error message is displayed.
 *
 * @param path The path of the directory to be created
 */
void DataWriter :: createDirectoryIfNotExists(const std::string& path) {
    struct stat info;

    // Check if the directory already exists
    if (stat(path.c_str(), &info) != 0) {
        // Directory doesn't exist - attempt to create it
        if (mkdir(path.c_str(), 0755) == -1) {
            std::cerr << "Error creating directory " << path
                      << ": " << strerror(errno) << std::endl;
        } else {
            std::cout << "Created directory: " << path << std::endl;
        }
    } else if (!(info.st_mode & S_IFDIR)) {
        // Path exists but is not a directory
        std::cerr << path << " exists but is not a Directory." << std::endl;
    } else {
        // Directory already exists
        std::cout << "Directory already exists: " << path << std::endl;
    }
}

/**
 * @brief Creates multiple directories if they don't already exist
 *
 * This function iterates through a list of directory paths and attempts to create
 * each one using the createDirectoryIfNotExists method.
 *
 * @param directories A vector containing the paths of directories to be created
 */
void DataWriter :: createDirectories(const std::vector<std::string>& directories) {
    // Create each directory in the provided list
    for (const auto& dir : directories) {
        createDirectoryIfNotExists(dir);
    }
}
/**
 * @brief Retrieves a list of redundant directories based on the specified redundancy level.
 *
 * This function calculates and returns a subset of output directories to be used for
 * redundant data storage. The number of directories returned is determined by the
 * redundancy level, ensuring data is written across multiple locations to prevent
 * single-point failures.
 *
 * @return std::vector<std::string> A vector containing paths to redundant directories
 */
std::vector<std::string> DataWriter::getRedundantDirectories() {
    std::vector<std::string> result;

    // If there are no output directories or redundancy level is zero, return empty result
    if (_outputDirectories.empty() || _redundancyLevel <= 0) {
        return result;
    }

    // Adjust redundancy level to not exceed available number of directories
    _redundancyLevel = std::min(_redundancyLevel, static_cast<int>(_outputDirectories.size()));

    // Create a copy of output directories and shuffle them to ensure random selection
    std::vector<std::string> shuffledDirs = _outputDirectories;
    std::random_device rd;  // Seed with a real random number if available
    std::mt19937 g(rd());   // Mersenne Twister engine randomized by rd()
    std::shuffle(shuffledDirs.begin(), shuffledDirs.end(), g);

    // Select the first 'redundancyLevel' directories from the shuffled list
    result.insert(result.end(),
                 shuffledDirs.begin(),
                 shuffledDirs.begin() + _redundancyLevel);

    return result;
}

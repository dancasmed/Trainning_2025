#include <iostream>
#include <atomic>
#include <thread>
#include <string>
#include <filesystem>
#include "LiveDataGenerator.h"
#include "ThreadSafeQueue.h"
#include "LiveDataCollector.h"
#include "BufferedWriter.h"
#include "DataWriter.h"
#include "HistoricalDataReader.h"

#include <fstream>
#include <fcntl.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zstd.h>


std::atomic<bool> runningFlag; ///< Global flag to control the execution state of all threads

const unsigned long BUFFER_SIZE = 10000; ///< Maximum number of MarketData records that can be stored in memory before flushing to disk
const int NUM_DATA_COLLECTORS = 10; ///< Number of parallel threads used to collect data from the queue
const long DATA_GENERATION_FREQUENCY = 5; ///< Frequency in milliseconds at which new market data is generated
const long COLLECTION_TIME = 500; ///< Total duration in milliseconds for which data collection will run
const bool ENABLE_COMPRESSION = true; ///< Enable or disable compression of the output data files
const int REDUNDANCY_LEVEL = 2; ///< Number of redundant copies to be maintained across different storage locations
const std::string OUTPUT_FILE_PREFIX = "data_"; ///< Prefix for the generated data files
const std::vector<std::string> OUTPUT_DIRECTORIES = { "remote1", "remote2", "remote3", "remote4" }; ///< List of output directories where the redundant copies will be stored. In a production environment, these should point to different storage locations.

/**
 * @brief Function to start and manage the data collection process for a single collector thread
 *
 * @param id Unique identifier for the collector thread
 * @param dataQueue Thread-safe queue containing market data records
 * @param bufferedWriter Writer object responsible for persisting the collected data
 */
void startDataCollector(int id, ThreadSafeQueue& dataQueue, BufferedWriter& bufferedWriter) {
    LiveDataCollector dc(&bufferedWriter);
    try {
        /**
         * Start the collection process. The collector will continuously take data from
         * the queue until the runningFlag is set to false.
         */
        dc.collect(dataQueue, runningFlag, id);
    } catch (const std::exception& e) {
        std::cerr << "Error in Data Collector with id " << id << ": " << e.what() << std::endl;
    }
}

/**
 * @brief Main function where the data collection system is initialized and executed
 *
 * @return int Exit status of the program
 */
int main() {
    //Object responsable for store the generated data to be consumes by data collectors
    ThreadSafeQueue dataQueue;
    
    //Object responsible for generating market data at regular intervals
    LiveDataGenerator dataGenerator(DATA_GENERATION_FREQUENCY, runningFlag);

    //Object responsible for writing the collected data to disk with optional compression
    DataWriter dataWriter(ENABLE_COMPRESSION, REDUNDANCY_LEVEL, OUTPUT_DIRECTORIES);

    //Object responsible for buffering and flushing collected data to disk
    BufferedWriter bufferedWriter(OUTPUT_FILE_PREFIX, BUFFER_SIZE, &dataWriter);
    HistoricalDataReader historicalDataReader(ENABLE_COMPRESSION, OUTPUT_DIRECTORIES);
    // Object responsable for reading and displaying historical data if available
    std::cout << "High-Performance Storage System Starting..." << std::endl;
    runningFlag = true;

    try {
        // Start the data generator thread
        std::thread dataGeneratorThread([&dataGenerator, &dataQueue]() { dataGenerator.start(dataQueue); });

        // Start multiple collector threads based on NUM_DATA_COLLECTORS
        std::vector<std::thread> dataCollectors;
        for (int i = 0; i < NUM_DATA_COLLECTORS; i++) {
            dataCollectors.emplace_back(startDataCollector, i, std::ref(dataQueue), std::ref(bufferedWriter));
        }
        
        // Main thread waits until the collection time has elapsed
        std::this_thread::sleep_for(std::chrono::milliseconds(COLLECTION_TIME));

        std::cout << "Stopping Data Generation and Data Collection ..." << std::endl;
        // Stop all threads and clean up resources
        runningFlag = false;

        dataGeneratorThread.join();
        for (auto& c : dataCollectors) {
            if (c.joinable()) {
                c.join();
            }
        }
        std::cout << "Live Data Collectors stoped." << std::endl;

        std::cout << "Flushing data buffer ..." << std::endl;
        bufferedWriter.~BufferedWriter();

    } catch (const std::exception& e) {
        /**
         * Handle any exceptions that occur during execution.
         * Set the running flag to false to ensure all child threads stop gracefully.
         */
        runningFlag = false;
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "Starting historical data reader..." << std::endl;
    
    // Read and display historical data if available
    for (const auto& dataRecord : historicalDataReader.readAll()) {
        auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(dataRecord.timestamp.time_since_epoch()).count();
        std::cout << "Historic data Reader - Time: " << milliseconds_since_epoch << " | Price: " << dataRecord.price << " | Volume: " << dataRecord.volume << std::endl;
    }
    return 0;
}

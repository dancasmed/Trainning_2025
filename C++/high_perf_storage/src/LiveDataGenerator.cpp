#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include "MarketData.h"
#include "ThreadSafeQueue.h"
#include "LiveDataGenerator.h"

using namespace std::chrono_literals;

/**
 * @brief Generates a MarketData object with random price, volume, and current timestamp.
 *
 * This function creates and populates a MarketData object using random number distributions
 * for price and volume, and sets the timestamp to the current time.
 *
 * @return MarketData A populated MarketData object with random values.
 */
MarketData LiveDataGenerator::generateMarketData()
{
    MarketData data;
    data.price = priceDist(gen);
    data.volume = volumeDist(gen);
    data.timestamp = std::chrono::high_resolution_clock::now();
    return data;
}

/**
 * @brief Constructor for LiveDataGenerator.
 *
 * Initializes the random number generator and distributions. Stores the frequency
 * for data generation and a reference to the running flag that controls the
 * generation loop.
 *
 * @param frequency The interval in milliseconds between consecutive data generations.
 * @param runningFlag A reference to an atomic boolean that signals whether the
 *                   generator should continue running or stop.
 */
LiveDataGenerator::LiveDataGenerator(long frequency, std::atomic<bool> &runningFlag)
{
    _runningFlag = &runningFlag;
    _frequency = frequency;
    gen.seed(rd());
    priceDist = std::uniform_real_distribution<>(100.0, 500.0);
    volumeDist = std::uniform_int_distribution<>(1, 1000);
}

/**
 * @brief Starts generating market data and enqueuing it to the provided queue.
 *
 * Begins a loop that periodically generates MarketData objects at the specified
 * frequency and adds them to the ThreadSafeQueue until the runningFlag is set to false.
 * The thread sleeps between iterations to maintain the generation frequency.
 *
 * @param queue A reference to the ThreadSafeQueue where generated data will be enqueued.
 */
void LiveDataGenerator::start(ThreadSafeQueue& queue)
{
    std::cout << "Starting Data Generation..." << std::endl;
    auto next = std::chrono::steady_clock::now();

    while (*_runningFlag == true) {
        next += std::chrono::milliseconds(_frequency);
        MarketData data = generateMarketData();
        queue.push(data);

        std::this_thread::sleep_until(next);
    }
    std::cout << "Live Data Generation finished." << std::endl;
}
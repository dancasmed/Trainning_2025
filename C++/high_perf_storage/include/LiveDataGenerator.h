#ifndef DATA_GENERATOR
#define DATA_GENERATOR

#include <random>
#include <atomic>
#include "MarketData.h"
#include "ThreadSafeQueue.h"

using namespace std::chrono_literals;

/**
 * @class LiveDataGenerator
 * @brief A class responsible for generating live market data at specified intervals
 *
 * This class generates simulated market data (price and volume) at a given frequency.
 * It runs in its own thread and sends generated data through a thread-safe queue.
 */
class LiveDataGenerator
{
private:
    std::atomic<bool>* _runningFlag;  ///< Flag to control the running state of the generator
    long _frequency;                 ///< Frequency of data generation per second

    std::random_device rd;           ///< Random device for seed generation
    std::mt19937 gen;                ///< Mersenne Twister random number engine
    std::uniform_real_distribution<> priceDist;  ///< Distribution for generating prices
    std::uniform_int_distribution<> volumeDist;  ///< Distribution for generating volumes

    /**
     * @brief Generates a single MarketData object with simulated values
     *
     * This method creates and returns a MarketData object containing random but realistic-looking
     * market data values.
     *
     * @return A MarketData object with the generated values
     */
    MarketData generateMarketData();

public:
    /**
     * @brief Constructor for LiveDataGenerator
     *
     * Initializes the generator with the specified frequency and running flag.
     *
     * @param frequency The rate at which data is generated, in Hz
     * @param runningFlag Reference to an atomic boolean controlling execution
     */
    LiveDataGenerator(long frequency, std::atomic<bool> &runningFlag);

    /**
     * @brief Destructor for LiveDataGenerator
     *
     * Default destructor as no special cleanup is required.
     */
    ~LiveDataGenerator() = default;

    /**
     * @brief Starts generating and sending market data to the provided queue
     *
     * This method begins the generation of market data at the specified frequency and sends each
     * generated MarketData object through the provided ThreadSafeQueue. The loop continues until
     * the runningFlag is set to false.
     *
     * @param queue The thread-safe queue to receive generated market data
     */
    void start(ThreadSafeQueue& queue);
};

#endif

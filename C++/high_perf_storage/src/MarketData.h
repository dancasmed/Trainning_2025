#ifndef MARKET_DATA
#define MARKET_DATA

#include <chrono>

/**
 * @brief Structure representing market data information
 *
 * This struct encapsulates essential market data elements including price, volume,
 * and timestamp of the market activity.
 */
struct MarketData
{
    double price; ///< The current market price (in smallest currency unit e.g., cents)
    int volume; ///< The number of units traded
    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp; ///< Timestamp when the market data was recorded
};

#endif

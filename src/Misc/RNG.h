#pragma once

namespace RNG
{

/**
 * @brief Get a random int in range [0, high)
 * 
 * @param high upper bound
 * @return int random int
 */
int RandomInt(int high);

/**
 * @brief Get a random int in range [low, high)
 * 
 * @param low lower bound
 * @param high upper bound
 * @return int random int
 */
int RandomInt(int low, int high);

/**
 * @brief Get a random double in range [0, 1)
 * 
 * @return double random double
 */
double RandomDouble();

/**
 * @brief Get a random double in range [low, high)
 * 
 * @param low lower bound
 * @param high upper bound
 * @return double random double
 */
double RandomDouble(double low, double high);

/**
 * @brief Decide whether the given chance shall be satisfied
 * 
 * @param threshold chance (accepted range is [0, 1))
 * @return true if satisfied
 */
bool Chance(double threshold);

} /* namespace RNG */
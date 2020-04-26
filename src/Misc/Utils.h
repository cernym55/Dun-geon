#pragma once

#include <string>

/**
 * @brief Checks whether the file with the given filename exists
 * 
 * @param filename filename
 * @return true if file exists
 */
bool fileExists(const std::string& filename);

/**
 * @brief Returns the absolute value of x
 * 
 * @param x
 * @return int |x|
 */
int Abs(int x);

/**
 * @brief Returns the lower of the two values
 * 
 * @tparam T comparable type
 * @param first first value
 * @param second second value
 * @return const T& the lower of the two
 */
template<typename T>
const T& Min(const T& first, const T& second)
{
    return first < second ? first : second;
}

/**
 * @brief Returns the higher of the two values
 * 
 * @tparam T comparable type
 * @param first first value
 * @param second second value
 * @return const T& the higher of the two
 */
template<typename T>
const T& Max(const T& first, const T& second)
{
    return first < second ? second : first;
}
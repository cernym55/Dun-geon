#pragma once

#include <string>

/**
 * @brief Major version number
 */
constexpr static const int GameVersionMajor = 0;

/**
 * @brief Minor version number
 */
constexpr static const int GameVersionMinor = 3;

/**
 * @brief Revision version number
 */
constexpr static const int GameVersionRevision = 4;

/**
 * @brief Current game version string
 */
static const std::string GameVersionString =
    "v" +
    std::to_string(GameVersionMajor) +
    "." +
    std::to_string(GameVersionMinor) +
    "." +
    std::to_string(GameVersionRevision);

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
 * @brief Shorten the string if it is above the maximum length
 * If the short string is above a certain length,
 * an ellipsis is inserted for indication.
 * 
 * @param str string
 * @param maxLength maximum length
 * @return std::string shortened string
 */
std::string ShortenString(const std::string& str, size_t maxLength);

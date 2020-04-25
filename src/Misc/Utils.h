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
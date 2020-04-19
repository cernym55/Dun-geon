#include "Utils.h"
#include <ctime>
#include <fstream>
#include <random>

std::mt19937 mt(std::time(0));

int RNG(int low, int high)
{
    std::uniform_int_distribution<int> randNum(low, high);
    return randNum(mt);
}

bool fileExists(const std::string& filename)
{
    std::ifstream f(filename.c_str());
    return f.good();
}

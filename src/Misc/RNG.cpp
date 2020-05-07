#include "RNG.h"
#include <random>

namespace RNG
{

/**
 * @brief Used to seed the MT
 */
static std::random_device rd;

/**
 * @brief The PRNG
 */
static std::mt19937 mt(rd());

int RandomInt(int high)
{
    std::uniform_int_distribution<int> dist(0, high - 1);
    return dist(mt);
}

int RandomInt(int low, int high)
{
    std::uniform_int_distribution<int> dist(low, high - 1);
    return dist(mt);
}

double RandomDouble()
{
    static std::uniform_real_distribution<double> dist;
    return dist(mt);
}

double RandomDouble(double low, double high)
{
    std::uniform_real_distribution<double> dist(low, high);
    return dist(mt);
}

bool Chance(double threshold)
{
    if (threshold >= 1.)
    {
        return true;
    }
    else if (threshold <= 0.)
    {
        return false;
    }

    return RandomDouble() < threshold;
}

} /* namespace RNG */
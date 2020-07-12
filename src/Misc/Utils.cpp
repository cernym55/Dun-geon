#include "Utils.h"
#include <fstream>
#include <string>

bool fileExists(const std::string& filename)
{
    std::ifstream f(filename.c_str());
    return f.good();
}

int Abs(int x)
{
    return x >= 0 ? x : -x;
}

std::string ShortenString(const std::string& str, size_t maxLength)
{
    if (str.size() <= maxLength) return str;
    std::string out = str;
    out.erase(out.begin() + maxLength, out.end());
    // Don't add an ellipsis if the string is very short
    if (maxLength > 5)
    {
        for (size_t i = 1; i <= 3; i++)
        {
            out[out.size() - i] = '.';
        }
    }
    return out;
}

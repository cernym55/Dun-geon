#include "Utils.h"
#include <chrono>
#include <fstream>
#include <string>
#include <thread>

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

std::vector<std::string> SplitStringIntoLines(std::string str, size_t maxWidth)
{
    std::vector<std::string> output;

    while (static_cast<size_t>(str.size()) > maxWidth)
    {
        int pos = maxWidth - 1;
        size_t newlinePos = str.substr(0, pos).find('\n');
        if (newlinePos != std::string::npos)
        {
            output.push_back(str.substr(0, newlinePos));
            str.erase(0, newlinePos + 1);
            continue;
        }

        while (!isspace(str.at(pos)))
        {
            // This will blow up if the message contains a single word longer than the maximum length
            // but that is a risk I'm willing to undertake
            pos--;
        }

        output.push_back(str.substr(0, pos));

        str.erase(0, pos + 1);
    }

    output.push_back(str);
    return output;
}

void Sleep(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
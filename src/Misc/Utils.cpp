#include "Utils.h"
#include <fstream>

bool fileExists(const std::string& filename)
{
    std::ifstream f(filename.c_str());
    return f.good();
}

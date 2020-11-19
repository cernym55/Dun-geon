#pragma once

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>

template<typename Printable>
void CheckStringConversion(const Printable& obj, const std::string& expected)
{
    std::ostringstream oss;
    oss << obj;
    BOOST_CHECK_EQUAL(oss.str(), expected);
}

#pragma once

#include <boost/test/unit_test.hpp>
#include <ncurses.h>
#include <sstream>
#include <string>

struct NcursesFixture
{
    NcursesFixture() { initscr(); }

    ~NcursesFixture() { endwin(); }
};

/**
 * @brief Assert that the operator<< string conversion of an object is equal to the expected value
 *
 * @tparam Printable type with operator<< string conversion
 * @param obj object
 * @param expected expected string representation
 */
template<typename Printable> void CheckStringConversion(const Printable& obj, const std::string& expected)
{
    std::ostringstream oss;
    oss << obj;
    BOOST_CHECK_EQUAL(oss.str(), expected);
}

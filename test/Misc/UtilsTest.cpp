#define BOOST_TEST_MODULE Misc.Utils
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "Misc/Utils.h"
#include <string>

BOOST_AUTO_TEST_CASE(fShortenString)
{
    std::string shortStr = "foo";
    std::string normalStr = "lorem ipsum";
    // Check trying to shorten an already short string
    BOOST_CHECK_EQUAL(ShortenString(shortStr, 6), shortStr);
    // Check shortening a normal string (has ellipsis)
    BOOST_CHECK_EQUAL(ShortenString(normalStr, 6), "lor...");
    // Check shortening a normal string a lot (no ellipsis)
    BOOST_CHECK_EQUAL(ShortenString(normalStr, 5), "lorem");
}

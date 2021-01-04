#define BOOST_TEST_MODULE UI.InputHandler
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "Helpers.h"
#include "UI/InputHandler.h"
#include <vector>

BOOST_FIXTURE_TEST_CASE(ReadKeypressSuccessCase, NcursesFixture)
{
    static const chtype Input = 'a';
    const std::vector<chtype> AllowedCharacters { Input, 'b' };

    std::optional<chtype> result;

    // Act
    {
        NcursesFixture f;

        ungetch(Input);
        result = UI::InputHandler::ReadKeypress(AllowedCharacters);
    }

    // Check we have received the correct input character
    BOOST_REQUIRE_EQUAL(result.has_value(), true);
    BOOST_CHECK_EQUAL(result.value(), Input);
}

BOOST_FIXTURE_TEST_CASE(ReadKeypressInvalidInputCase, NcursesFixture)
{
    static const chtype Input = 'a';
    const std::vector<chtype> AllowedCharacters { Input + 1 };

    std::optional<chtype> result;

    // Act
    {
        NcursesFixture f;

        ungetch(Input);
        result = UI::InputHandler::ReadKeypress(AllowedCharacters);
    }

    // Check that we have not received a valid input
    BOOST_CHECK_EQUAL(result.has_value(), false);
}
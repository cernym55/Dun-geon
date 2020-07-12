#define BOOST_TEST_MODULE Misc.Direction
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "Helpers.h"
#include "Misc/Direction.h"

BOOST_AUTO_TEST_CASE(Statics)
{
    // Check that the static members are initialized correctly
    BOOST_CHECK_EQUAL(Direction::Up(), Direction::Value::Up);
    BOOST_CHECK_EQUAL(Direction::Right(), Direction::Value::Right);
    BOOST_CHECK_EQUAL(Direction::Down(), Direction::Value::Down);
    BOOST_CHECK_EQUAL(Direction::Left(), Direction::Value::Left);
    BOOST_CHECK_EQUAL(Direction::None(), Direction::Value::None);

    // Check the "All" collection has the correct size and order of elements
    BOOST_CHECK_EQUAL(Direction::All.size(), 4);
    for (size_t i = 0; i < Direction::All.size(); i++)
    {
        BOOST_CHECK_EQUAL(Direction::All[i], static_cast<Direction::Value>(i));
    }
}

BOOST_AUTO_TEST_CASE(Constructors)
{
    // Check that constructors assign the correct values
    Direction defaultDirection;
    BOOST_CHECK_EQUAL(defaultDirection(), Direction::Value::None);

    Direction nonDefaultDirection(Direction::Value::Right);
    BOOST_CHECK_EQUAL(nonDefaultDirection(), Direction::Value::Right);

    Direction copy(nonDefaultDirection);
    BOOST_CHECK_EQUAL(copy(), Direction::Value::Right);
}

BOOST_AUTO_TEST_CASE(Opposite)
{
    // Check that Opposite returns the correct values
    BOOST_CHECK_EQUAL(Direction(Direction::Value::Up).Opposite()(),    Direction::Value::Down);
    BOOST_CHECK_EQUAL(Direction(Direction::Value::Right).Opposite()(), Direction::Value::Left);
    BOOST_CHECK_EQUAL(Direction(Direction::Value::Down).Opposite()(),  Direction::Value::Up);
    BOOST_CHECK_EQUAL(Direction(Direction::Value::Left).Opposite()(),  Direction::Value::Right);
    BOOST_CHECK_EQUAL(Direction(Direction::Value::None).Opposite()(),  Direction::Value::None);
}

BOOST_AUTO_TEST_CASE(ToInt)
{
    // Check that ToInt returns the correct values
    BOOST_CHECK_EQUAL(Direction(Direction::Value::Up).ToInt(), 0);
    BOOST_CHECK_EQUAL(Direction(Direction::Value::Right).ToInt(), 1);
    BOOST_CHECK_EQUAL(Direction(Direction::Value::Down).ToInt(), 2);
    BOOST_CHECK_EQUAL(Direction(Direction::Value::Left).ToInt(), 3);
    BOOST_CHECK_EQUAL(Direction(Direction::Value::None).ToInt(), -1);
}

BOOST_AUTO_TEST_CASE(Operators)
{
    // Check operator==
    Direction a(Direction::Value::Up);
    Direction a2(Direction::Value::Up);
    BOOST_CHECK(a == a2);
    // Check operator!=
    Direction b(Direction::Value::Down);
    BOOST_CHECK(a != b);

    // Check operator<<
    CheckStringConversion(Direction(Direction::Value::Up), "up");
    CheckStringConversion(Direction(Direction::Value::Right), "right");
    CheckStringConversion(Direction(Direction::Value::Down), "down");
    CheckStringConversion(Direction(Direction::Value::Left), "left");
    CheckStringConversion(Direction(Direction::Value::None), "[none]");
}

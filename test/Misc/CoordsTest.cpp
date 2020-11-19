#define BOOST_TEST_MODULE Misc.Coords
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "Helpers.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Misc/Exceptions.h"
#include <algorithm>
#include <sstream>

BOOST_AUTO_TEST_CASE(Constructors)
{
    // Check that constructors assign the correct values
    Coords defaultCoords;
    BOOST_CHECK_EQUAL(defaultCoords, Coords(0, 0));

    Coords nonDefaultCoords(2, 4);
    BOOST_CHECK_EQUAL(nonDefaultCoords, Coords(2, 4));

    Coords copy(nonDefaultCoords);
    BOOST_CHECK_EQUAL(copy, Coords(2, 4));
}

BOOST_AUTO_TEST_CASE(Adjacent)
{
    Coords coords(2, 4);
    Coords adjacent = coords.Adjacent(Direction::Up);
    BOOST_CHECK_EQUAL(adjacent.X, coords.X);
    BOOST_CHECK_EQUAL(adjacent.Y, coords.Y - 1);
}

BOOST_AUTO_TEST_CASE(Move)
{
    Coords coords(2, 4);
    coords.Move(Direction::Up);
    BOOST_CHECK_EQUAL(coords.X, 2);
    BOOST_CHECK_EQUAL(coords.Y, 3);
}

BOOST_AUTO_TEST_CASE(SharesAxis)
{
    // Check both axes for correct results
    Coords coords(2, 4);
    Coords verticallyAlignedCoords(2, 6);
    Coords horizontallyAlignedCoords(5, 4);
    Coords unalignedCoords(5, 6);

    BOOST_CHECK(coords.SharesAxis(verticallyAlignedCoords));
    BOOST_CHECK(coords.SharesAxis(horizontallyAlignedCoords));
    BOOST_CHECK(!coords.SharesAxis(unalignedCoords));
}

BOOST_AUTO_TEST_CASE(StraightPath)
{
    Coords from(2, 4);
    Coords to(7, 4);
    auto path = from.StraightPath(to);
    // Check size
    BOOST_CHECK_EQUAL(path.size(), 6);

    // Check all elements of the path are present
    for (int x = 2; x <= 7; x++)
    {
        BOOST_CHECK(std::find(path.begin(), path.end(), Coords(x, 4)) != path.end());
    }

    // Check that a non-straight path throws an InvalidPositionException
    Coords invalidTo(7, 5);
    BOOST_CHECK_THROW(from.StraightPath(invalidTo), InvalidPositionException);
}

BOOST_AUTO_TEST_CASE(Distance)
{
    // Check the distance is correct when from and to share an axis
    Coords from(2, 4);
    Coords toSharesAxis(2, 8);
    BOOST_CHECK_EQUAL(from.Distance(toSharesAxis), 4);

    // Check combined distances also work
    Coords to(5, 8);
    BOOST_CHECK_EQUAL(from.Distance(to), 7);
}

BOOST_AUTO_TEST_CASE(Operators)
{
    // Check operator==
    Coords a(2, 4);
    Coords a2(2, 4);
    BOOST_CHECK(a == a2);
    // Check operator!=
    Coords b(1, 1);
    BOOST_CHECK(a != b);
    // Check operator+
    BOOST_CHECK_EQUAL(a + b, Coords(3, 5));
    // Check operator-
    BOOST_CHECK_EQUAL(a - b, Coords(1, 3));

    // Check operator+=
    a += b;
    BOOST_CHECK_EQUAL(a, Coords(3, 5));
    // Check operator-=
    a -= b;
    BOOST_CHECK_EQUAL(a, a2);

    // Check operator<<
    a = {2, 4};
    CheckStringConversion(a, "[2, 4]");
}

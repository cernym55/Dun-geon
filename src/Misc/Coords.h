#pragma once

#include "Direction.h"
#include <cstdint>
#include <iostream>
#include <vector>

/**
 * @brief Helper class for storing 2D coordinates
 */
class Coords
{
public:
    /**
     * @brief Size type for coords, distances, dimensions etc.
     */
    using Scalar = std::int16_t;

    /**
     * @brief X coordinate
     */
    Scalar X;

    /**
     * @brief Y coordinate
     */
    Scalar Y;

    /**
     * @brief Constructor
     */
    Coords();

    /**
     * @brief Constructor
     *
     * @param xCoord X coordinate
     * @param yCoord Y coordinate
     */
    Coords(Scalar xCoord, Scalar yCoord);

    /**
     * @brief Get the adjacent coordinates in the given direction
     *
     * @param dir direction
     * @return Coords adjacent coordinates
     */
    Coords Adjacent(Direction dir) const;

    /**
     * @brief Shift the coordinates by one in the given direction
     *
     * @param dir direction
     * @return Coords& this
     */
    Coords& Move(Direction dir);

    /**
     * @brief Check if these coords are on the same horizontal or vertical axis
     *
     * @param there target coords
     * @return true if shares axis
     */
    bool SharesAxis(Coords there) const;

    /**
     * @brief Return a vector of coords in a straight line from here to there
     *
     * @param there target coords
     * @return std::vector<Coords> coords forming path
     */
    std::vector<Coords> StraightPath(Coords there) const;

    /**
     * @brief Gets the combined X and Y distance from here to there
     *
     * @param there target coords
     * @return Scalar distance
     */
    Scalar CombinedDistance(Coords there) const;

    inline Coords& operator+=(const Coords& r) { X += r.X; Y += r.Y; return *this; }

    inline Coords& operator-=(const Coords& r) { X -= r.X; Y -= r.Y; return *this; }
};

inline Coords operator+(Coords l, const Coords& r) { return l += r; }

inline Coords operator-(Coords l, const Coords& r) { return l -= r; }

inline bool operator==(const Coords& l, const Coords& r)
{
    return l.X == r.X && l.Y == r.Y;
}

inline bool operator!=(const Coords& l, const Coords& r)
{
    return !(l == r);
}

std::ostream& operator<<(std::ostream& os, const Coords& coords);

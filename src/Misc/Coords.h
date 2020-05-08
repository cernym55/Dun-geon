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
     * @brief 
     * 
     */
    using Scalar = std::int16_t;

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
     * @brief Equals operator overload
     * 
     * @param r 
     * @return true if equal
     */
    bool operator==(const Coords& r) const;

    /**
     * @brief Get the X coordinate
     * 
     * @return Scalar X coordinate
     */
    Scalar GetX() const;

    /**
     * @brief Set the X coordinate
     * 
     * @param value new value
     */
    void SetX(Scalar value);

    /**
     * @brief Get the Y coordinate
     * 
     * @return Scalar Y coordinate
     */
    Scalar GetY() const;

    /**
     * @brief Set the Y coordinate
     * 
     * @param value new value
     */
    void SetY(Scalar value);

    /**
     * @brief Get the adjacent coordinates in the given direction
     * 
     * @param dir direction
     * @return Coords adjacent coordinates
     */
    Coords GetAdjacent(Direction dir) const;

    /**
     * @brief Shift the coordinates by one in the given direction
     * 
     * @param dir direction
     * @return Coords& this
     */
    Coords& MoveInDirection(Direction dir);

    /**
     * @brief Check if these coords are on the same horizontal or vertical axis
     * 
     * @param there target coords
     * @return true if shares axis
     */
    bool SharesAxisWith(Coords there) const;

    /**
     * @brief Return a vector of coords in a straight line from here to there
     * 
     * @param there target coords
     * @return std::vector<Coords> coords forming path
     */
    std::vector<Coords> StraightPathTo(Coords there) const;

    /**
     * @brief Gets the combined X and Y distance from here to there
     * 
     * @param there target coords
     * @return Scalar distance
     */
    Scalar CombinedDistanceFrom(Coords there) const;

    /**
     * @brief Operator << overload
     * 
     * @param os output stream
     * @param coords coords
     * @return std::ostream& output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Coords& coords);

private:
    Scalar m_XCoord;
    Scalar m_YCoord;
};
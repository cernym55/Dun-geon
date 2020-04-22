#pragma once

#include "Direction.h"
#include <iostream>

/**
 * @brief Helper class for storing 2D coordinates
 */
class Coords
{
public:
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
    Coords(size_t xCoord, size_t yCoord);

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
     * @return size_t 
     */
    size_t GetX() const;

    /**
     * @brief Set the X coordinate
     * 
     * @param value new value
     */
    void SetX(size_t value);

    /**
     * @brief Get the Y coordinate
     * 
     * @return size_t Y coordinate
     */
    size_t GetY() const;

    /**
     * @brief Set the Y coordinate
     * 
     * @param value new value
     */
    void SetY(size_t value);

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
     * @brief Operator << overload
     * 
     * @param os output stream
     * @param coords coords
     * @return std::ostream& output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Coords& coords);

private:
    size_t m_XCoord;
    size_t m_YCoord;
};
#pragma once

#include <array>
#include <ostream>

/**
 * @brief Represents the four directions on a map
 */
class Direction
{
public:
    /**
     * @brief Direction value
     * Allowed values are Up, Right, Down, Left and a neutral None value.
     * By convention Directions shall be listed clockwise starting with Up.
     */
    enum class Value
    {
        /**
         * @brief None
         */
        None = -1,

        /**
         * @brief Up
         */
        Up = 0,

        /**
         * @brief Right
         */
        Right = 1,

        /**
         * @brief Down
         */
        Down = 2,

        /**
         * @brief Left
         */
        Left = 3
    };

    /**
     * @brief Constructor
     */
    Direction();

    /**
     * @brief Constructor
     * 
     * @param value value
     */
    Direction(Value value);

    /**
     * @brief Operator== overload
     * 
     * @param r
     * @return true if equal
     */
    bool operator==(const Direction& r) const;

    /**
     * @brief Operator!= overload
     * 
     * @param r 
     * @return true if not equal
     */
    bool operator!=(const Direction& r) const;

    /**
     * @brief Operator() overload
     * Returns the contained value.
     * 
     * @return Value value
     */
    Value operator()() const;

    /**
     * @brief Operator< overload
     * 
     * @param r
     * @return true if (int)(*this)() < (int)r()
     */
    bool operator<(const Direction& r) const;

    /**
     * @brief Get the opposite direction
     * 
     * @return Direction opposite direction
     */
    Direction Opposite() const;

    /**
     * @brief Return the int representation of the enum value
     * 
     * @return int int value
     */
    int ToInt() const;

    /**
     * @brief Return a Direction object with the value Up
     * 
     * @return Direction Up direction
     */
    static Direction Up();

    /**
     * @brief Return a Direction object with the value Right
     * 
     * @return Direction Right direction
     */
    static Direction Right();

    /**
     * @brief Return a Direction object with the value Down
     * 
     * @return Direction Down direction
     */
    static Direction Down();

    /**
     * @brief Return a Direction object with the value Left
     * 
     * @return Direction Left direction
     */
    static Direction Left();

    /**
     * @brief Return a Direction object with the value None
     * 
     * @return Direction "None" direction
     */
    static Direction None();

    /**
     * @brief Return an iterable array containing all four directions
     * 
     * @return std::array<Direction, 4> collection of directions
     */
    static std::array<Direction, 4> All();

    /**
     * @brief Operator<< overload
     * 
     * @param os output stream
     * @param r 
     * @return std::ostream& output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Direction& r);

private:
    Value m_Value;
};
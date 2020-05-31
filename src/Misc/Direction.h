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
     * @brief "None" direction
     */
    static const Direction None;

    /**
     * @brief Up direction
     */
    static const Direction Up;

    /**
     * @brief Right direction
     */
    static const Direction Right;

    /**
     * @brief Down direction
     */
    static const Direction Down;

    /**
     * @brief Left direction
     */
    static const Direction Left;

    /**
     * @brief Array of all directions except "None"
     */
    static const std::array<Direction, 4> All;

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
    inline int ToInt() const { return static_cast<int>(m_Value); }

    inline Value operator()() const { return m_Value; }


private:
    Value m_Value;
};

inline bool operator<(const Direction& l, const Direction& r) { return l.ToInt() < r.ToInt(); }

inline bool operator==(const Direction& l, const Direction& r) { return l() == r(); }

inline bool operator!=(const Direction& l, const Direction& r) { return !(l == r); }

std::ostream& operator<<(std::ostream& os, const Direction& r);

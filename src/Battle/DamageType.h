#pragma once

#include <array>
#include <ncurses.h>
#include <string>

namespace Battle
{

/**
 * @brief Damage type smart enum
 */
class DamageType
{
public:
    /**
     * @brief Damage type value
     */
    enum class Value
    {
        Physical = 0,
        Fire     = 1,
        Frost    = 2,
        Poison   = 3,
        Magic    = 4
    };

    /**
     * @brief Physical damage type
     */
    static const DamageType Physical;

    /**
     * @brief Fire damage type
     */
    static const DamageType Fire;

    /**
     * @brief Frost damage type
     */
    static const DamageType Frost;

    /**
     * @brief Poison damage type
     */
    static const DamageType Poison;

    /**
     * @brief Magic damage type
     */
    static const DamageType Magic;

    /**
     * @brief Array of all damage types
     */
    static const std::array<DamageType, 5> All;

    /**
     * @brief Constructor
     *
     * @param value value
     */
    DamageType(Value value);

    /**
     * @brief Return the int representation of the enum value
     *
     * @return int int value
     */
    inline int ToInt() const { return static_cast<int>(m_Value); }

    /**
     * @brief Get the underlying enum value
     *
     * @return Value value
     */
    inline Value operator()() const { return m_Value; }

    /**
     * @brief Get the string representation of the type
     * 
     * @return std::string string representation
     */
    std::string ToString() const;

    /**
     * @brief Print the name in color at the current position
     * 
     * @param window window to print to
     */
    void PrintColor(WINDOW* window) const;

private:
    Value m_Value;
};

inline bool operator==(const DamageType& l, const DamageType& r)
{
    return l() == r();
}

inline bool operator!=(const DamageType& l, const DamageType& r)
{
    return !(l == r);
}

} /* namespace Battle */
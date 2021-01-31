#pragma once

#include "DamageType.h"

namespace Battle
{

/**
 * @brief Instance of damage
 *
 */
class DamageInstance
{
public:
    /**
     * @brief Constructor
     *
     * @param value damage value
     * @param damageType damage type
     */
    DamageInstance(int value, DamageType damageType);

    /**
     * @brief Get the value
     * 
     * @return int value
     */
    inline int GetValue() const { return m_Value; }

    /**
     * @brief Get the damage type
     * 
     * @return DamageType damage type
     */
    inline DamageType GetDamageType() const { return m_DamageType; }

    /**
     * @brief Print the value in color at current position
     * 
     * @param window window to print to
     */
    void PrintInColor(WINDOW* window) const;

private:
    int m_Value;
    DamageType m_DamageType;
};

} /* namespace Battle */
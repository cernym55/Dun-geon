#pragma once

#include "Character.h"
#include "Misc/Direction.h"
#include <ncurses.h>
#include <string>

namespace Entities
{

/**
 * @brief Player character class
 */
class Player : public Character
{
public:
    Direction LastMoveDirection;

    /**
     * @brief Constructor
     *
     * @param initialCoords initial coords
     * @param name name
     * @param icon icon (default: set to first character of name)
     */
    Player(Coords initialCoords,
           const std::string& name,
           chtype icon = 0);

    /**
     * @brief Get XP
     */
    int GetXP() const;

    /**
     * @brief Get XP needed to level up
     */
    int GetXPToLevelUp() const;

    /**
     * @brief Get dun
     */
    int GetDun() const;

private:
    int m_XP;
    int m_XPToLevelUp;
    int m_Dun;
};

} /* namespace Entities */

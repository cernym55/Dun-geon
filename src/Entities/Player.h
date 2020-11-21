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
    /**
     * @brief Constructor
     *
     * @param name name
     * @param initialStats initial stats
     * @param icon icon (default: set to first character of name)
     */
    Player(const std::string& name,
           Stats initialStats,
           chtype icon = 0);

    /**
     * @brief Set the direction of the last move
     *
     * @param dir direction
     */
    void SetLastMoveDirection(Direction dir);

    /**
     * @brief Set the coordinates
     *
     * @param coords coords
     */
    void SetCoords(Coords coords);

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

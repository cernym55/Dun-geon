#pragma once

#include "Entity.h"
#include "Misc/Direction.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include <array>
#include <ncurses.h>
#include <vector>

namespace Entities
{


/**
 * @brief Game character with stats and movement
 */
class Character : public Entity
{
public:
    struct Stats
    {
        int level, health, healthMax, mana, manaMax, vigor, valor, haste, magic;
    };

    /**
     * @brief Constructor
     *
     * @param name name
     * @param description description (default: empty)
     * @param icon icon (default: set to first character of name)
     * @param isBlocking blocking attribute (default: true)
     */
    Character(const std::string& name,
              const std::string& description = "",
              chtype icon = 0,
              bool isBlocking = true);

    /**
     * @brief Move the character position in the given direction
     *
     * @param dir direction
     */
    void Move(Direction dir);

    /**
     * @brief Get the direction of the last move the character performed
     *
     * @return Direction last move direction
     */
    Direction GetLastMoveDirection() const;

    const Stats& GetStats() const;

protected:
    Direction m_LastMoveDirection;
    Stats m_Stats;
};

} /* namespace Entities */

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

struct Stats
{
    int level, XP, XPToNextLevel, dun, health, healthMax, mana, manaMax, vigor, valor, haste, magic;
};

/**
 * @brief Game character with stats and movement
 */
class Character : public Entity
{
public:
    Character(const std::string& name,
              const std::string& description = "",
              chtype icon = 0,
              bool isBlocking = true);

    void Move(Direction dir);

    Direction GetLastMoveDirection() const;

    Stats& GetStats();
    const Stats& GetStats() const;

protected:
    Direction m_LastMoveDirection;
    Stats m_Stats;
};

} /* namespace Entities */

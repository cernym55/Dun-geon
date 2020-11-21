#include "Player.h"
#include "Misc/Direction.h"
#include <ncurses.h>

namespace Entities
{

Player::Player(const std::string& name,
               Stats initialStats,
               chtype icon)
    : Character(name, "", icon, true), m_XP(0), m_XPToLevelUp(100), m_Dun(0)
{
    m_Stats = initialStats;
}

void Player::SetLastMoveDirection(Direction dir)
{
    m_LastMoveDirection = dir;
}

void Player::SetCoords(Coords coords)
{
    m_Coords = coords;
}

int Player::GetXP() const
{
    return m_XP;
}

int Player::GetXPToLevelUp() const
{
    return m_XPToLevelUp;
}

int Player::GetDun() const
{
    return m_Dun;
}

} /* namespace Entities */

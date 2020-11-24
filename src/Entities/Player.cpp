#include "Player.h"
#include "Misc/Direction.h"
#include <ncurses.h>

namespace Entities
{

Player::Player(Coords initialCoords,
               const std::string& name,
               chtype icon)
    : Character(initialCoords, name, "", icon), m_XP(0), m_XPToLevelUp(100), m_Dun(0)
{
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

void Player::Move(Direction dir)
{
    Character::Move(dir);
}

} /* namespace Entities */

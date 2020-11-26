#include "Player.h"
#include "Misc/Direction.h"
#include "Misc/Exceptions.h"
#include <ncurses.h>

namespace Entities
{

Player::Player(const std::string& name,
               chtype icon)
    : Character(name, "", icon), m_XP(0), m_XPToLevelUp(100), m_Dun(0)
{
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

Direction Player::GetNextMove(const EntityManager& entityManager)
{
    throw NotSupportedException("Attempted to make unsupported call: Player::GetNextMove()");
}

} /* namespace Entities */

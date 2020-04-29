#include "Player.h"
#include "Misc/Direction.h"
#include <ncurses.h>

namespace Entities
{

Player::Player(const std::string& name,
               chtype icon)
    : Character(name, "", icon, true)
{
}

void Player::SetLastMoveDirection(Direction dir)
{
    m_LastMoveDirection = dir;
}

} /* namespace Entities */
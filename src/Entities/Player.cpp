#include "Player.h"
#include "Misc/Direction.h"

namespace Entities
{

Player::Player(const std::string& name,
               const std::string& description,
               char icon,
               bool isBlocking)
    : Character(name, description, icon, isBlocking)
{
}

void Player::SetLastMoveDirection(Direction dir)
{
    m_LastMoveDirection = dir;
}

} /* namespace Entities */
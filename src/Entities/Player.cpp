#include "Player.h"
#include "Misc/Direction.h"
#include "Worlds/Room.h"

namespace Entities
{

Player::Player(const std::string& name,
               const std::string& description,
               char icon,
               bool isBlocking)
    : Character(name, description, icon, isBlocking)
{
}

void Player::SwitchCurrentRoom(Direction dir)
{
    currentRoom = &currentRoom->GetNeighbor(dir);
}

} /* namespace Entities */
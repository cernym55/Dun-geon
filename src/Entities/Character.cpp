#include "Character.h"
#include "Misc/Direction.h"
#include "Worlds/Room.h"
#include <string>

namespace Entities
{

Character::Character(const std::string& name,
                     const std::string& description,
                     char icon,
                     bool isBlocking)
    : Entity(name, description, icon, isBlocking)
{
}

void Character::move(Direction dir)
{
    m_Coords.MoveInDirection(dir);
    lastMove = dir;
}

bool Character::CanMove(Direction dir) const
{
    if (dir == Direction::None()) return true;

    const Worlds::Field* targetField = collision(dir);
    if (targetField != nullptr && targetField->GetForegroundEntity() == nullptr)
    {
        return true;
    }

    return false;
}

// // Returns pointers to the 4 fields adjacent to the character
// std::array<Worlds::Field*, 4> Character::collision()
// {
//     return { { currentRoom->GetFieldAt(m_Coords.GetAdjacent(Entities::up)),
//                currentRoom->getField(posX + 1, posY),
//                currentRoom->getField(posX, posY + 1),
//                currentRoom->getField(posX - 1, posY) } };
// }

// Returns pointers to the 4 fields adjacent to the character
const std::array<const Worlds::Field*, 4> Character::collision() const
{
    return { { m_Coords.GetY() != 0 ? &currentRoom->GetFieldAt(m_Coords.GetAdjacent(Direction::Up())) : nullptr,
               m_Coords.GetX() != currentRoom->GetWidth() - 1 ? &currentRoom->GetFieldAt(m_Coords.GetAdjacent(Direction::Right())) : nullptr,
               m_Coords.GetY() != currentRoom->GetHeight() - 1 ? &currentRoom->GetFieldAt(m_Coords.GetAdjacent(Direction::Down())) : nullptr,
               m_Coords.GetX() != 0 ? &currentRoom->GetFieldAt(m_Coords.GetAdjacent(Direction::Left())) : nullptr } };
}

// Returns pointer to a specific field adjacent to the character
const Worlds::Field* Character::collision(Direction direction) const
{
    return direction == Direction::None() ? nullptr : collision()[(int)direction()];
}

const Entity* Character::touching() const
{
    return collision(lastMove) == nullptr ? nullptr : collision(lastMove)->GetForegroundEntity();
}

Stats& Character::getStats()
{
    return stats;
}

const Stats& Character::getStats() const
{
    return stats;
}

const Worlds::Room* Character::getCurrentRoom() const
{
    return currentRoom;
}

Direction Character::getLastMove()
{
    return lastMove;
}

void Character::setLastMove(Direction direction)
{
    lastMove = direction;
}

} /* namespace Entities */

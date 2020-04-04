#include "character.h"
#include "room.h"

void Character::move(Direction direction)
{
    switch (direction)
    {
    case up:
        posY--;
        break;
    case right:
        posX++;
        break;
    case down:
        posY++;
        break;
    case left:
        posX--;
        break;
    case nil:
        break;
    }
    lastMove = direction;
}

// Returns pointers to the 4 fields adjacent to the character
std::array<Field*, 4> Character::collision()
{
    return { { currentRoom->getField(posX, posY - 1),
               currentRoom->getField(posX + 1, posY),
               currentRoom->getField(posX, posY + 1),
               currentRoom->getField(posX - 1, posY) } };
}

// Returns pointer to a specific field adjacent to the character
Field* Character::collision(Direction direction)
{
    return direction == nil ? currentRoom->getField(posX, posY) : collision()[direction];
}

// Returns whatever object the character has just walked up to (or null)
Entity* Character::touching()
{
    return lastMove == nil || collision(lastMove) == nullptr ? nullptr : collision(lastMove)->content;
}

Stats& Character::getStats()
{
    return stats;
}

Room* Character::getCurrentRoom()
{
    return currentRoom;
}

void Character::setCurrentRoom(Room* roomPtr)
{
    currentRoom = roomPtr;
}

Direction Character::getLastMove()
{
    return lastMove;
}

void Character::setLastMove(Direction direction)
{
    lastMove = direction;
}

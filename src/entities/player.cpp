#include "player.h"
#include "world/room.h"
#include "world/world.h"

// TODO: separate room transitions into another function
// TODO: do not move the player, handle that elsewhere using the return bool
bool Player::attemptMove(Direction dir)
{
    Field* colField = collision(dir);
    if (dir != nil && colField != nullptr && !colField->isWall)
    {
        if (colField->content == nullptr || !colField->content->isBlocking())
        {
            move(dir);
            return true;
        }
    }
    else if (dir != nil && colField == nullptr)
    {
        Direction nextEntranceDir = (Direction)((dir + 2) % 4); // opposite direction
        currentRoom = currentRoom->getNeighbor(dir);
        if (!currentRoom->generated())
        {
            switch (nextEntranceDir)
            {
            case up:
                currentRoom->generate(randLayout, true);
                break;
            case right:
                currentRoom->generate(randLayout, false, true);
                break;
            case down:
                currentRoom->generate(randLayout, false, false, true);
                break;
            case left:
                currentRoom->generate(randLayout, false, false, false, true);
                break;
            default:
                break;
            }
        }
        posX = currentRoom->getEntrance(nextEntranceDir)->posX;
        posY = currentRoom->getEntrance(nextEntranceDir)->posY;
        return true;
    }
    return false;
}

#include "world.h"
#include "room.h"

namespace Worlds
{

World::World(int num, Entities::Player* pl)
{
    playerPtr = pl;
    number = num;
    nextRoomNum = 1;
    rooms = std::vector<std::vector<Room*>>(WORLD_SIZE, std::vector<Room*>(WORLD_SIZE, nullptr));
    for (int i = 0; i < rooms.size(); i++)
    {
        for (int j = 0; j < rooms[i].size(); j++)
        {
            rooms[i][j] = new Room(this);
            rooms[i][j]->setPosX(i);
            rooms[i][j]->setPosY(j);
        }
    }
    // must be done after all rooms are constructed
    for (int i = 0; i < rooms.size(); i++)
    {
        for (int j = 0; j < rooms[i].size(); j++)
        {
            rooms[i][j]->loadNeighbors();
        }
    }
}

World::~World()
{
    for (int i = 0; i < rooms.size(); i++)
    {
        for (int j = 0; j < rooms[i].size(); j++)
        {
            delete rooms[i][j];
        }
    }
}

int World::getNum()
{
    return number;
}

Room* World::getRoom(int x, int y)
{
    return x < 0 || x >= WORLD_SIZE || y < 0 || y >= WORLD_SIZE ? nullptr : rooms[x][y];
}

std::vector<std::vector<Room*>>& World::getRooms()
{
    return rooms;
}

int World::getNextRoomNum()
{
    return nextRoomNum;
}

void World::setNextRoomNum(int value)
{
    nextRoomNum = value;
}

Entities::Player* World::getPlayer()
{
    return playerPtr;
}

} /* namespace Worlds */
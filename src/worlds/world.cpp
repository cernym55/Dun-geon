#include "world.h"
#include "world_manager.h"
#include "room.h"

namespace Worlds
{

World::World(WorldManager& worldManager, int num)
    : m_WorldManager(worldManager), m_WorldNumber(num), m_NextRoomNum(1)
{
    m_Rooms = std::vector<std::vector<Room*>>(WORLD_SIZE, std::vector<Room*>(WORLD_SIZE, nullptr));
    for (int i = 0; i < m_Rooms.size(); i++)
    {
        for (int j = 0; j < m_Rooms[i].size(); j++)
        {
            m_Rooms[i][j] = new Room(m_WorldManager, *this);
            m_Rooms[i][j]->setPosX(i);
            m_Rooms[i][j]->setPosY(j);
        }
    }
    // must be done after all rooms are constructed
    for (int i = 0; i < m_Rooms.size(); i++)
    {
        for (int j = 0; j < m_Rooms[i].size(); j++)
        {
            m_Rooms[i][j]->loadNeighbors();
        }
    }
}

World::~World()
{
    for (int i = 0; i < m_Rooms.size(); i++)
    {
        for (int j = 0; j < m_Rooms[i].size(); j++)
        {
            delete m_Rooms[i][j];
        }
    }
}

int World::GetWorldNumber() const
{
    return m_WorldNumber;
}

Room* World::GetRoom(int x, int y)
{
    return x < 0 || x >= WORLD_SIZE || y < 0 || y >= WORLD_SIZE ? nullptr : m_Rooms[x][y];
}

std::vector<std::vector<Room*>>& World::GetRooms()
{
    return m_Rooms;
}

int World::GetNextRoomNum()
{
    return m_NextRoomNum;
}

void World::SetNextRoomNum(int value)
{
    m_NextRoomNum = value;
}

} /* namespace Worlds */
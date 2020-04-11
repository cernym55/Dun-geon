#pragma once

#include "entities/player.h"
#include <vector>

#define WORLD_SIZE 15

namespace Worlds
{

class WorldManager;
class Room; //TODO: remove forward declaration

class World
{
public:
    World(WorldManager& worldManager, int num);
    World(const World& world);
    ~World();
    int GetWorldNumber() const;
    Room* GetRoom(int x, int y);
    std::vector<std::vector<Room*>>& GetRooms();
    int GetNextRoomNum();
    void SetNextRoomNum(int value);

private:
    WorldManager& m_WorldManager;
    int m_WorldNumber;
    int m_NextRoomNum;
    std::vector<std::vector<Room*>> m_Rooms;
};

} /* namespace Worlds */
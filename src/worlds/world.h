#pragma once

#include "entities/player.h"
#include <vector>

#define WORLD_SIZE 15

namespace Worlds
{

class Room; //TODO: remove forward declaration

class World
{
private:
    int number;
    int nextRoomNum;
    std::vector<std::vector<Room*>> rooms;
    Entities::Player* playerPtr;

public:
    World(int num, Entities::Player* pl);
    ~World();
    int getNum();
    Room* getRoom(int x, int y);
    std::vector<std::vector<Room*>>& getRooms();
    int getNextRoomNum();
    void setNextRoomNum(int value);
    Entities::Player* getPlayer();
};

} /* namespace Worlds */
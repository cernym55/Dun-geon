#pragma once

#include <vector>

#define WORLD_SIZE 15

class Room;
class Player;

class World
{
private:
    int number;
    int nextRoomNum;
    std::vector<std::vector<Room*>> rooms;
    Player* playerPtr;

public:
    World(int num, Player* pl);
    ~World();
    int getNum();
    Room* getRoom(int x, int y);
    std::vector<std::vector<Room*>>& getRooms();
    int getNextRoomNum();
    void setNextRoomNum(int value);
    Player* getPlayer();
};

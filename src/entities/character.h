#pragma once

#include "entity.h"
#include <array>
#include <vector>

struct Field;
class Room;

enum Direction
{
    up,
    right,
    down,
    left,
    nil = -1
};

struct Stats
{
    int level, XP, XPToNextLevel, dun, health, healthMax, mana, manaMax, vigor, valor, haste, magic;
    //std::vector<Skill> skills; //TODO: add this when skills are done
    //std::vector<Item> inventory; //TODO: add this when items are done
};

class Character : public Entity
{
protected:
    Stats stats;
    Room* currentRoom;
    Direction lastMove;

public:
    virtual ~Character(){};
    void move(Direction direction);
    std::array<Field*, 4> collision();
    Field* collision(Direction direction);
    Entity* touching();
    Stats& getStats();
    Room* getCurrentRoom();
    void setCurrentRoom(Room* roomPtr);
    Direction getLastMove();
    void setLastMove(Direction direction);
};
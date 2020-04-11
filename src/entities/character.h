#pragma once

#include "entity.h"
#include <array>
#include <vector>

namespace Worlds
{

class Room;
struct Field;

} /* namespace Worlds */

namespace Entities
{

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
    Worlds::Room* currentRoom;
    Direction lastMove;

public:
    virtual ~Character(){};
    void move(Direction direction);
    std::array<Worlds::Field*, 4> collision();
    Worlds::Field* collision(Direction direction);
    Entity* touching();
    Stats& getStats();
    const Stats& getStats() const;
    const Worlds::Room* getCurrentRoom() const;
    void setCurrentRoom(Worlds::Room* roomPtr);
    Direction getLastMove();
    void setLastMove(Direction direction);
};

} /* namespace Entities */
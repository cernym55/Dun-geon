#pragma once

#include "Entity.h"
#include "Misc/Direction.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include <array>
#include <vector>

namespace Entities
{

struct Stats
{
    int level, XP, XPToNextLevel, dun, health, healthMax, mana, manaMax, vigor, valor, haste, magic;
    //std::vector<Skill> skills; //TODO: add this when skills are done
    //std::vector<Item> inventory; //TODO: add this when items are done
};

class Character : public Entity
{
public:
    /**
     * @brief Constructor
     * 
     * @param name name
     * @param description description (default: empty)
     * @param icon icon (default: set to first character of name)
     * @param isBlocking blocking attribute (default: true)
     */
    Character(const std::string& name,
              const std::string& description = "",
              char icon = 0,
              bool isBlocking = true);

    virtual ~Character(){};
    void move(Direction dir);
    bool CanMove(Direction dir) const;
    // std::array<Worlds::Field*, 4> collision();
    const std::array<const Worlds::Field*, 4> collision() const;
    const Worlds::Field* collision(Direction direction) const;
    // Entity* touching();
    const Entity* touching() const;
    Stats& getStats();
    const Stats& getStats() const;
    const Worlds::Room* getCurrentRoom() const;
    Direction getLastMove();
    void setLastMove(Direction direction);

protected:
    Stats stats;
    const Worlds::Room* currentRoom;
    Direction lastMove;
};

} /* namespace Entities */
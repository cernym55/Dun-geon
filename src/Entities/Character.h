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
    
    Character(const Character&) = delete;

    Character(Character&&) = delete;

    Character& operator=(const Character&) = delete;

    Character& operator=(Character&&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~Character() = default;

    /**
     * @brief Move the character position in the given direction
     * 
     * @param dir direction
     */
    void Move(Direction dir);

    /**
     * @brief Get the direction of the last move the character performed
     * 
     * @return Direction last move direction
     */
    Direction GetLastMoveDirection() const;

    Stats& GetStats();
    const Stats& GetStats() const;

protected:
    Direction m_LastMoveDirection;
    Stats m_Stats;
};

} /* namespace Entities */
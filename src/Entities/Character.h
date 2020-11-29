#pragma once

#include "NPC/Behavior/IMovement.h"
#include "Entity.h"
#include "Misc/Direction.h"
#include <array>
#include <memory>
#include <ncurses.h>
#include <vector>

namespace Entities
{

class EntityManager;

/**
 * @brief Game character with stats and movement
 */
class Character : public Entity
{
public:
    /**
     * @brief Collection of traditional RPG stats
     */
    struct Stats
    {
        int Level, Health, MaxHealth, Mana, MaxMana, Vigor, Valor, Haste, Magic;
    };

    /**
     * @brief Constructor
     *
     * @param name name
     * @param description description (default: empty)
     * @param icon icon (default: set to first character of name)
     * @param initialStats initial stats (default: arbitrary values)
     * @param isBlocking blocking attribute (default: true)
     */
    Character(const std::string& name,
              const std::string& description = "",
              chtype icon = 0,
              Stats initialStats = { 1, 10, 10, 5, 10, 5, 5, 5, 10 },
              bool isBlocking = true);

    /**
     * @brief Destructor
     */
    virtual ~Character() = default;

    /**
     * @brief Get the direction of the next move
     *
     * @param entityManager entity manager
     */
    virtual Direction GetNextMove(const EntityManager& entityManager) override;
    
    /**
     * @brief Is this fightable?
     * 
     * @return true if fightable
     */
    virtual bool Fightable() const override;

    /**
     * @brief Get the stats collection
     *
     * @return Stats stats
     */
    const Stats& GetStats() const;

protected:
    Stats m_Stats;
    std::unique_ptr<NPC::Behavior::IMovement> m_MovementBehavior;
};

} /* namespace Entities */

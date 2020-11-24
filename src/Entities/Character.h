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
     * @param initialCoords initial coords
     * @param name name
     * @param description description (default: empty)
     * @param icon icon (default: set to first character of name)
     * @param initialStats initial stats (default: arbitrary values)
     * @param isBlocking blocking attribute (default: true)
     */
    Character(Coords initialCoords,
              const std::string& name,
              const std::string& description = "",
              chtype icon = 0,
              Stats initialStats = { 1, 10, 10, 5, 10, 5, 5, 5, 10 },
              bool isBlocking = true);

    /**
     * @brief Perform movement behavior
     *
     * @param entityManager entity manager
     */
    virtual void PerformMovement(const EntityManager& entityManager) override;

    /**
     * @brief Get the direction of the last move the character performed
     *
     * @return Direction last move direction
     */
    Direction GetLastMoveDirection() const;

    /**
     * @brief Get the stats collection
     *
     * @return Stats stats
     */
    const Stats& GetStats() const;

protected:
    Direction m_LastMoveDirection;
    Stats m_Stats;
    std::unique_ptr<NPC::Behavior::IMovement> m_MovementBehavior;

    /**
     * @brief Move the character position in the given direction
     *
     * @param dir direction
     */
    void Move(Direction dir);
};

} /* namespace Entities */

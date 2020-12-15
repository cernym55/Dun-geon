#pragma once

#include "Battle/Skill.h"
#include "Entity.h"
#include "Misc/Direction.h"
#include "NPC/Behavior/IMovement.h"
#include "Stats.h"
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
              chtype icon                    = 0,
              Stats initialStats             = { 1, 30, 30, 10, 10, 25, 20, 20, 10 },
              bool isBlocking                = true);

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

    /**
     * @brief Get the skillset
     *
     * @return auto& skillset
     */
    inline auto& GetSkills() { return m_Skillset; }

    /**
     * @brief Add a skill to the character's skillset
     *
     * @tparam SkillType skill class
     * @param level skill level to add
     */
    template<typename SkillClass> void GrantSkill(int level = 1) { m_Skillset.emplace_back(new SkillClass()); }

protected:
    Stats m_Stats;
    std::unique_ptr<NPC::Behavior::IMovement> m_MovementBehavior;
    std::vector<std::unique_ptr<Battle::Skill>> m_Skillset;
};

} /* namespace Entities */

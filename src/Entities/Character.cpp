#include "NPC/Behavior/WanderingMovement.h"
#include "EntityManager.h"
#include "Character.h"
#include "Misc/Direction.h"
#include "Worlds/Room.h"
#include <ncurses.h>
#include <string>

namespace Entities
{

Character::Character(const std::string& name,
                     const std::string& description,
                     chtype icon,
                     Stats initialStats,
                     bool isBlocking)
    : Entity(name, description, icon, isBlocking),
    m_Stats(initialStats),
    m_MovementBehavior(std::make_unique<NPC::Behavior::WanderingMovement>(*this))
{
}

Direction Character::GetNextMove(const EntityManager& entityManager)
{
    return m_MovementBehavior->GetNextStep(entityManager);
}

const Character::Stats& Character::GetStats() const
{
    return m_Stats;
}

} /* namespace Entities */

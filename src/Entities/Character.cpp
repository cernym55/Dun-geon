#include "Character.h"
#include "EntityManager.h"
#include "Misc/Direction.h"
#include "NPC/Behavior/WanderingMovement.h"
#include "Worlds/Room.h"
#include <cmath>
#include <ncurses.h>
#include <string>

namespace Entities
{

Character::Character(const std::string& name,
                     const std::string& description,
                     chtype icon,
                     int baseXPReward,
                     Stats initialStats,
                     bool isBlocking)
    : Entity(name, description, icon, isBlocking),
      m_BaseXPReward(baseXPReward),
      m_Stats(initialStats),
      m_MovementBehavior(std::make_unique<NPC::Behavior::WanderingMovement>(*this))
{
}

Direction Character::GetNextMove(const EntityManager& entityManager)
{
    return m_MovementBehavior->GetNextStep(entityManager);
}

bool Character::Fightable() const
{
    return true;
}

int Character::CalculateXPReward() const
{
    return lround(m_BaseXPReward * sqrt(m_Stats.Level));
}

} /* namespace Entities */

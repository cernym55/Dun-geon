#include "NPC/Behavior/WanderingMovement.h"
#include "EntityManager.h"
#include "Character.h"
#include "Misc/Direction.h"
#include "Worlds/Room.h"
#include <ncurses.h>
#include <string>

namespace Entities
{

Character::Character(Coords initialCoords,
                     const std::string& name,
                     const std::string& description,
                     chtype icon,
                     Stats initialStats,
                     bool isBlocking)
    : Entity(name, description, icon, isBlocking),
    m_LastMoveDirection(Direction::None),
    m_Stats(initialStats),
    m_MovementBehavior(std::make_unique<NPC::Behavior::WanderingMovement>(*this))
{
    m_Coords = initialCoords;
}

void Character::PerformMovement(const EntityManager& entityManager)
{
    Move(m_MovementBehavior->GetNextStep(entityManager));
}

Direction Character::GetLastMoveDirection() const
{
    return m_LastMoveDirection;
}

const Character::Stats& Character::GetStats() const
{
    return m_Stats;
}

void Character::Move(Direction dir)
{
    m_Coords.Move(dir);
    m_LastMoveDirection = dir;
}

} /* namespace Entities */

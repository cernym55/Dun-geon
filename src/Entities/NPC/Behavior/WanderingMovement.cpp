#include "WanderingMovement.h"
#include "EntityManager.h"
#include "Misc/RNG.h"
#include <algorithm>

namespace Entities::NPC::Behavior
{

WanderingMovement::WanderingMovement(Character& character)
    : m_Character(character)
{
}

Direction WanderingMovement::GetNextStep(const EntityManager& entityManager)
{
    double rng = RNG::RandomDouble();
    // Chance to keep the same movement
    if (rng < 0.7)
    {
        if (entityManager.CanEntityMove(m_Character, m_LastMoveDirection) &&
        !entityManager.RoomOf(m_Character).IsAtRoomEdge(entityManager.CoordsOf(m_Character).Adjacent(m_LastMoveDirection), m_LastMoveDirection))
        {
            return m_LastMoveDirection;
        }
    }
    // Chance to stop and stay still
    else if (rng < 0.85 && m_LastMoveDirection != Direction::None)
    {
        m_LastMoveDirection = Direction::None;
        return Direction::None;
    }

    // Where else can we go?
    std::vector<Direction> allowedDirections;
    std::copy_if(Direction::All.begin(), Direction::All.end(), std::back_inserter(allowedDirections),
                 [&](const auto& dir)
                 {
                     return
                         entityManager.CanEntityMove(m_Character, dir) &&
                         !entityManager.RoomOf(m_Character).IsAtRoomEdge(entityManager.CoordsOf(m_Character).Adjacent(dir), dir) &&
                         m_LastMoveDirection.Opposite() != dir;
                 });

    if (allowedDirections.empty())
    {
        return Direction::None;
    }

    // Pick a random allowed direction
    Direction randomNewDirection = allowedDirections[RNG::RandomInt(allowedDirections.size())];

    m_LastMoveDirection = randomNewDirection;
    return randomNewDirection;
}

} /* namespace Entities::NPC::Behavior */
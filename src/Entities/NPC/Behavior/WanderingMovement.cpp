#include "WanderingMovement.h"
#include "EntityManager.h"
#include "Misc/RNG.h"

namespace Entities::NPC::Behavior
{

WanderingMovement::WanderingMovement(Character& character)
    : m_Character(character)
{
}

Direction WanderingMovement::GetNextStep(const EntityManager& entityManager)
{
    if (!RNG::Chance(0.5)) return Direction::None;

    Direction candidateDirection = Direction::All[RNG::RandomInt(4)];

    if (entityManager.CanCharacterMove(m_Character, candidateDirection))
    {
        return candidateDirection;
    }
    else
    {
        return Direction::None;
    }
    
}

} /* namespace Entities::NPC::Behavior */
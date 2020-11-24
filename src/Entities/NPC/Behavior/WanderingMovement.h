#pragma once

#include "IMovement.h"
#include "Character.h"

namespace Entities::NPC::Behavior
{

/**
 * @brief Wandering movement pattern
 */
class WanderingMovement : public IMovement
{
public:
    /**
     * @brief Constructor
     *
     * @param character character
     */
    WanderingMovement(Character& character);

    /**
     * @brief Get the next step direction
     *
     * @param entityManager entity manager
     * @return Direction next step
     */
    virtual Direction GetNextStep(const EntityManager& entityManager) override;

private:
    Character& m_Character;
};

} /* namespace Entities::NPC::Behavior */
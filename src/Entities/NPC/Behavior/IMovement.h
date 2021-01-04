#pragma once

#include "Direction.h"

namespace Entities { class EntityManager; }

namespace Entities::NPC::Behavior
{

/**
 * @brief Interface for movement patterns
 */
class IMovement
{
public:
    /**
     * @brief Destructor
     */
    virtual ~IMovement() = default;

    /**
     * @brief Get the next step direction
     *
     * @param entityManager entity manager
     * @return Direction next step
     */
    virtual Direction GetNextStep(const EntityManager& entityManager) = 0;
};

} /* namespace Entities::NPC::Behavior */
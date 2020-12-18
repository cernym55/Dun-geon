#pragma once

#include "Player.h"
#include "NPCCharacter.h"
#include "Worlds/WorldManager.h"
#include <memory>

namespace Entities
{
class EntityManager;
}

namespace Entities::NPC
{

/**
 * @brief Manages NPC generation
 */
class NPCGenerator
{
public:
    /**
     * @brief Constructor
     *
     * @param entityManager entity manager
     * @param player player
     * @param worldManager world manager
     */
    NPCGenerator(EntityManager& entityManager, const Player& player, const Worlds::WorldManager& worldManager);

    /**
     * @brief Generate a random enemy NPC
     * 
     * @return std::unique_ptr<NPCCharacter> new NPC
     */
    std::unique_ptr<NPCCharacter> GenerateRandomEnemy();

private:
    EntityManager& m_EntityManager;
    const Player& m_Player;
    const Worlds::WorldManager& m_WorldManager;
};

} /* namespace Entities::NPC */
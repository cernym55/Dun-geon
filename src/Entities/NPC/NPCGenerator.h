#pragma once

#include "Character.h"
#include "Player.h"
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
     * @return std::unique_ptr<Character> new NPC
     */
    std::unique_ptr<Character> CreateRandomEnemy();

private:
    EntityManager& m_EntityManager;
    const Player& m_Player;
    const Worlds::WorldManager& m_WorldManager;

    /**
     * @brief Generate a random enemy NPC with the given level
     *
     * @param level enemy level
     * @return std::unique_ptr<Character> new NPC
     */
    std::unique_ptr<Character> CreateRandomEnemyAtLevel(int level);
};

} /* namespace Entities::NPC */
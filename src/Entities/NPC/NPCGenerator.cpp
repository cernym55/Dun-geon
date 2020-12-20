#include "NPCGenerator.h"
#include "EntityManager.h"
#include "NPCCollection.h"

namespace Entities::NPC
{

NPCGenerator::NPCGenerator(EntityManager& entityManager, const Player& player, const Worlds::WorldManager& worldManager)
    : m_EntityManager(entityManager),
      m_Player(player),
      m_WorldManager(worldManager)
{
}

std::unique_ptr<Character> NPCGenerator::CreateRandomEnemy()
{
    return CreateRandomEnemyAtLevel(m_Player.GetStats().Level);
}

std::unique_ptr<Character> NPCGenerator::CreateRandomEnemyAtLevel(int level)
{
    // Decide what it will be
    return std::unique_ptr<Character>(new FadingSpirit(level));
}

} /* namespace Entities::NPC */
#include "NPCGenerator.h"
#include "EntityManager.h"
#include "Misc/RNG.h"
#include "NPCCollection.h"
#include <algorithm>

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
    int enemyLevel = std::clamp(m_Player.GetStats().Level + RNG::RandomInt(-2, 3), 1, 100);
    return CreateRandomEnemyAtLevel(enemyLevel);
}

std::unique_ptr<Character> NPCGenerator::CreateRandomEnemyAtLevel(int level)
{
    // Decide what it will be
    return std::unique_ptr<Character>(new FadingSpirit(level));
}

} /* namespace Entities::NPC */
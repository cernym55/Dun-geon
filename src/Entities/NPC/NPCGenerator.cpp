#include "NPCGenerator.h"
#include "EntityManager.h"

namespace Entities::NPC
{

NPCGenerator::NPCGenerator(EntityManager& entityManager, const Player& player, const Worlds::WorldManager& worldManager)
    : m_EntityManager(entityManager),
      m_Player(player),
      m_WorldManager(worldManager)
{
}

std::unique_ptr<NPCCharacter> NPCGenerator::GenerateRandomEnemy()
{
    // TODO: Make this really random
    return std::unique_ptr<NPCCharacter>(new NPCCharacter({}, "Jackson", "Human"));
}

} /* namespace Entities::NPC */
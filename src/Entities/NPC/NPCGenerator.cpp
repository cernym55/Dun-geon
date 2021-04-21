#include "NPCGenerator.h"
#include "EntityManager.h"
#include "Misc/RNG.h"
#include "NPCCollection.h"
#include <algorithm>

namespace Entities::NPC
{

static const std::vector<NPCCollection::Type> World1EnemyTypes { NPCCollection::Type::FadingSpirit,
                                                                 NPCCollection::Type::Rat };

NPCGenerator::NPCGenerator(EntityManager& entityManager, const Player& player, const Worlds::WorldManager& worldManager)
    : m_EntityManager(entityManager),
      m_Player(player),
      m_WorldManager(worldManager)
{
}

std::unique_ptr<Character> NPCGenerator::CreateRandomEnemy()
{
    // For now this simple algorithm based on distance and world number will do
    int roomDistance = m_WorldManager.CurrentRoom().GetCoords().Distance(
        Coords { Worlds::World::CenterPos, Worlds::World::CenterPos });
    if (roomDistance > 14)
    {
        roomDistance = 14;
    }
    int worldBonus = 15 * (m_WorldManager.CurrentWorld().GetWorldNumber() - 1);
    int enemyLevel = std::clamp(roomDistance + RNG::RandomInt(-1, 2) + worldBonus, 1, Entities::LevelCap);

    return CreateRandomEnemyAtLevel(enemyLevel);
}

std::unique_ptr<Character> NPCGenerator::CreateRandomEnemyAtLevel(int level)
{
    NPCCollection::Type selectedType = World1EnemyTypes[RNG::RandomInt(World1EnemyTypes.size())];

    switch (selectedType)
    {
    case NPCCollection::Type::FadingSpirit:
        return std::unique_ptr<Character>(new NPCCollection::FadingSpirit(level));
    case NPCCollection::Type::Rat:
        return std::unique_ptr<Character>(new NPCCollection::Rat(level));
    default:
        return nullptr;
    }
}

} /* namespace Entities::NPC */
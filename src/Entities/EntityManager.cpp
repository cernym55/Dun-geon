#include "EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include "Worlds/WorldManager.h"
#include <memory>

namespace Entities
{

EntityManager::EntityManager(Worlds::WorldManager& worldManager, Player& player)
    : m_WorldManager(worldManager),
      m_Player(player)
{
}

void EntityManager::StoreEntity(Worlds::Room& room, Entity& entity)
{
    // Storing pointers is safe because both rooms and entities are tied to the world's lifespan
    m_EntityStorage[&room].emplace_back(&entity);
}

void EntityManager::UpdateEntitiesInCurrentRoom()
{
    UpdateEntitiesInRoom(m_WorldManager.GetCurrentRoom());
}

void EntityManager::UpdateEntitiesInRoom(Worlds::Room& room)
{
    if (m_WorldManager.IsCurrentRoom(room))
    {
        room.GetFieldAt(m_Player.GetCoords()).VacateForeground();
    }
    VacateStoredEntityFieldsInRoom(room);

    //TODO: Implement NPC behavior
    //TODO: Resolve movement conflicts between player and NPCs or two NPCs

    PlaceStoredEntitiesInRoom(room);
    if (m_WorldManager.IsCurrentRoom(room))
    {
        room.GetFieldAt(m_Player.GetCoords()).PlaceEntity(m_Player);
    }
}

void EntityManager::PlaceStoredEntitiesInRoom(Worlds::Room& room)
{
    for (auto& entity : m_EntityStorage[&room])
    {
        auto& field = room.GetFieldAt(entity->GetCoords());
        field.PlaceEntity(*entity);
    }
}

void EntityManager::VacateStoredEntityFieldsInRoom(Worlds::Room& room)
{
    for (auto& entity : m_EntityStorage[&room])
    {
        auto& field = room.GetFieldAt(entity->GetCoords());
        entity->IsBlocking() ? field.VacateForeground() : field.VacateBackground();
    }
}

} /* namespace Entities */
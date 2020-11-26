#include "EntityManager.h"
#include "Character.h"
#include "Entity.h"
#include "Misc/Direction.h"
#include "Misc/RNG.h"
#include "NPC/Human.h"
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
    const auto& world = const_cast<const Worlds::WorldManager&>(m_WorldManager).CurrentWorld();
    m_EntityCoords[&m_Player] = { static_cast<Coords::Scalar>(world.StartingRoom().GetWidth() / 2),
                             static_cast<Coords::Scalar>(world.StartingRoom().GetHeight() / 2) };
    Place(m_Player, m_WorldManager.CurrentRoom());
}

void EntityManager::SpawnEntity(Worlds::Room& room, Coords spawnPosition)
{
    auto& newEntity = m_EntityStorage[&room].emplace_back(new NPC::Human("Jackson"));
    m_EntityCoords[newEntity.get()] = spawnPosition;
    Place(*newEntity, room);
}

void EntityManager::Store(Worlds::Room& room, Entity& entity)
{
    // Storing pointers is safe because both rooms and entities are tied to the world's lifespan
    m_EntityStorage[&room].emplace_back(&entity);
}

bool EntityManager::TryMovePlayer(Direction dir)
{
    if (CanEntityMove(m_Player, dir))
    {
        Pluck(m_Player, m_WorldManager.CurrentRoom());
        m_EntityCoords[&m_Player].Move(dir);
        m_Player.LastMoveDirection = dir;
        Place(m_Player, m_WorldManager.CurrentRoom());
        CycleCurrentRoom();
        return true;
    }
    else if (m_WorldManager.CurrentRoom().IsAtRoomEdge(m_EntityCoords[&m_Player], dir))
    {
        Direction nextRoomEntranceDir = dir.Opposite();
        Pluck(m_Player, m_WorldManager.CurrentRoom());
        Coords offset = m_EntityCoords[&m_Player] - m_WorldManager.CurrentRoom().Entrance(dir)->GetCoords();
        Worlds::Room& nextRoom = m_WorldManager.SwitchRoom(dir);
        Coords newCoords = nextRoom
                               .Entrance(nextRoomEntranceDir)
                               ->GetCoords() +
                           offset;
        m_EntityCoords[&m_Player] = newCoords;
        m_Player.LastMoveDirection = dir;
        Place(m_Player, nextRoom);

        // Randomly spawn NPCs in an accessible location just for fun
        // TODO: remove
        if (RNG::Chance(0.1) && m_EntityStorage[&nextRoom].size() < 5)
        {
            Coords spawnPosition;
            while (!nextRoom.FieldAt(spawnPosition).IsAccessible() || nextRoom.FieldAt(spawnPosition).ForegroundEntity() != nullptr)
            {
                spawnPosition = Coords(RNG::RandomInt(nextRoom.GetWidth()), RNG::RandomInt(nextRoom.GetHeight()));
            }
            SpawnEntity(nextRoom, spawnPosition);
        }

        CycleCurrentRoom();
        return true;
    }

    m_Player.LastMoveDirection = dir;
    return false;
}

const Entity* EntityManager::Approaching(const Entity& entity, Direction dir) const
{
    const Worlds::Field* approachedField = AdjacentField(entity, dir);
    return approachedField != nullptr ? approachedField->ForegroundEntity() : nullptr;
}

bool EntityManager::CanEntityMove(const Entity& entity, Direction dir) const
{
    if (dir == Direction::None) return true;

    const Worlds::Field* targetField = AdjacentField(entity, dir);
    if (targetField != nullptr && targetField->ForegroundEntity() == nullptr)
    {
        return true;
    }

    return false;
}

Coords EntityManager::CoordsOf(const Entity& entity) const
{
    return m_EntityCoords.at(&entity);
}

void EntityManager::MoveEntity(Entity& entity, Direction dir)
{
    if (CanEntityMove(entity, dir))
    {
        Pluck(entity, m_WorldManager.CurrentRoom());
        m_EntityCoords[&entity].Move(dir);
        Place(entity, m_WorldManager.CurrentRoom());
    }
}

const std::array<const Worlds::Field*, 4> EntityManager::AdjacentFields(
    const Entity& entity) const
{
    Coords coords = m_EntityCoords.at(&entity);
    const Worlds::Room& currentRoom = m_WorldManager.CurrentRoom();
    std::array<const Worlds::Field*, 4> fields;
    for (const auto& dir : Direction::All)
    {
        fields[dir.ToInt()] = !currentRoom.IsAtRoomEdge(coords, dir)
                                  ? &currentRoom.FieldAt(coords.Adjacent(dir))
                                  : nullptr;
    }
    return fields;
}

const Worlds::Field* EntityManager::AdjacentField(
    const Entity& entity,
    Direction direction) const
{
    return direction != Direction::None
               ? AdjacentFields(entity)[direction.ToInt()]
               : nullptr;
}

void EntityManager::Cycle(Worlds::Room& room)
{
    for (auto& entity : m_EntityStorage[&room])
    {
        MoveEntity(*entity, entity->GetNextMove(*this));
        // TODO: Add additional behaviors
    }
}

void EntityManager::Place(Entity& entity, Worlds::Room& room)
{
    auto& field = room.FieldAt(m_EntityCoords[&entity]);
    field.PlaceEntity(entity);
}

void EntityManager::Pluck(Entity& entity, Worlds::Room& room)
{
    auto& field = room.FieldAt(m_EntityCoords[&entity]);
    entity.IsBlocking() ? field.VacateForeground() : field.VacateBackground();
}

} /* namespace Entities */

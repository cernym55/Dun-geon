#include "EntityManager.h"
#include "Character.h"
#include "Entity.h"
#include "Misc/Direction.h"
#include "Misc/RNG.h"
#include "Player.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include "Worlds/WorldManager.h"
#include <algorithm>
#include <memory>

namespace Entities
{

EntityManager::EntityManager(Worlds::WorldManager& worldManager, Player& player)
    : m_WorldManager(worldManager),
      m_Player(player),
      m_NPCGenerator(*this, player, worldManager)
{
    const auto& world = m_WorldManager.CurrentWorld();
    m_EntityCoords[&m_Player] = { static_cast<Coords::Scalar>(world.StartingRoom().GetWidth() / 2),
                             static_cast<Coords::Scalar>(world.StartingRoom().GetHeight() / 2) };
    Place(m_Player, m_WorldManager.CurrentRoom());
}

void EntityManager::KillEntity(Entity& entity)
{
    auto room = m_RoomsByEntity.at(&entity);
    auto& storage = m_EntityStorage.at(room);
    
    auto it = std::find_if(storage.begin(), storage.end(), [&](const auto& p)
    {
        return p.get() == &entity;
    });

    if (it != storage.end())
    {
        Pluck(entity, *room);
        m_RoomsByEntity.erase(it->get());
        m_EntityCoords.erase(it->get());
        storage.erase(it);
    }
}

void EntityManager::Store(Worlds::Room& room, std::unique_ptr<Entity>&& entity, Coords coords)
{
    m_EntityStorage[&room].push_back(std::move(entity));
    auto& movedEntity                  = m_EntityStorage[&room].back();
    m_EntityCoords[movedEntity.get()]  = coords;
    m_RoomsByEntity[movedEntity.get()] = &room;
    Place(*movedEntity, room);
}

bool EntityManager::TryMovePlayer(Direction dir)
{
    if (CanEntityMove(m_Player, dir))
    {
        Pluck(m_Player, m_WorldManager.CurrentRoom());
        m_EntityCoords[&m_Player].Move(dir);
        m_Player.FacingDirection = dir;
        Place(m_Player, m_WorldManager.CurrentRoom());
        CycleCurrentRoom();
        return true;
    }
    else if (m_WorldManager.CurrentRoom().IsAtRoomEdge(m_EntityCoords[&m_Player], dir))
    {
        Direction nextRoomEntranceDir = dir.Opposite();
        bool nextRoomExists           = m_WorldManager.CurrentRoom().HasNeighbor(dir);
        Pluck(m_Player, m_WorldManager.CurrentRoom());
        Coords offset = m_EntityCoords[&m_Player] - m_WorldManager.CurrentRoom().Entrance(dir)->GetCoords();
        Worlds::Room& nextRoom = m_WorldManager.SwitchRoom(dir);
        Coords newCoords = nextRoom
                               .Entrance(nextRoomEntranceDir)
                               ->GetCoords() +
                           offset;
        m_EntityCoords[&m_Player] = newCoords;
        m_Player.FacingDirection = dir;
        Place(m_Player, nextRoom);

        // (Re)populate the room with NPCs
        bool firstEntry = !nextRoomExists;
        PopulateRoom(nextRoom, firstEntry);

        CycleCurrentRoom();
        return true;
    }

    m_Player.FacingDirection = dir;
    return false;
}

Entity* EntityManager::Approaching(const Entity& entity, Direction dir)
{
    if (m_EntityStorage.count(&m_WorldManager.CurrentRoom()) == 0)
    {
        return nullptr;
    }

    Coords targetCoords = CoordsOf(entity).Adjacent(dir);
    for (auto& entity : m_EntityStorage.at(&m_WorldManager.CurrentRoom()))
    {
        if (CoordsOf(*entity) == targetCoords) return entity.get();
    }

    return nullptr;
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

const Worlds::Room& EntityManager::RoomOf(const Entity& entity) const
{
    return *m_RoomsByEntity.at(&entity);
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
    if (m_EntityStorage.count(&room) == 0)
    {
        return;
    }

    for (auto& entity : m_EntityStorage.at(&room))
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

void EntityManager::PopulateRoom(Worlds::Room& room, bool firstEntry)
{
    int entityCount = 0;
    double rng      = RNG::RandomDouble();
    double postRng  = RNG::RandomDouble();

    if (firstEntry)
    {
        // First time population
        if (m_EntityStorage.count(&room) != 0)
        {
            return;
        }

        if (rng < room.GetNPCSpawnChance())
        {
            // Spawn nothing now, but make it possible to repopulate the room
            m_EntityStorage[&room];
            return;
        }

        if (postRng > 0.45)
        {
            entityCount = 1;
        }
        // Limit higher spawn counts by room size
        else if (postRng > 0.15 || room.AccessibleFieldCount() <= 90)
        {
            entityCount = 2;
        }
        else if (postRng > 0.05 || room.AccessibleFieldCount() <= 120)
        {
            entityCount = 3;
        }
        else
        {
            entityCount = 4;
        }
    }
    else
    {
        // Repopulation
        if (m_EntityStorage.count(&room) == 0 || !m_EntityStorage[&room].empty())
        {
            return;
        }

        if (rng < room.GetNPCSpawnChance() * 0.75)
        {
            // We tried to repopulate and got 0, delete the container so we don't try again
            m_EntityStorage.erase(&room);
            return;
        }

        if (postRng > 0.25)
        {
            entityCount = 1;
        }
        else
        {
            entityCount = 2;
        }
    }

    for (int i = 0; i < entityCount; i++)
    {
        Coords spawnPosition;
        while (!room.FieldAt(spawnPosition).IsAccessible() || room.FieldAt(spawnPosition).ForegroundEntity() != nullptr)
        {
            spawnPosition = Coords(RNG::RandomInt(room.GetWidth() - 2) + 1, RNG::RandomInt(room.GetHeight() - 2) + 1);
        }

        auto newEntity = m_NPCGenerator.CreateRandomEnemy();
        Store(room, std::move(newEntity), spawnPosition);
    }
}

} /* namespace Entities */

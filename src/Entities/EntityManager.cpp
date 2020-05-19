#include "EntityManager.h"
#include "Character.h"
#include "Entity.h"
#include "Misc/Direction.h"
#include "Player.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include "Worlds/WorldManager.h"
#include <memory>

namespace Entities
{

/**
 * @brief Constructor
 *
 * @param WorldManager world manager
 * @param player player
 */
EntityManager::EntityManager(Worlds::WorldManager& worldManager, Player& player)
    : m_WorldManager(worldManager),
      m_Player(player)
{
}

/**
 * @brief Take ownership of an entity and assign it to this room's storage
 *
 * @param room room
 * @param entity entity
 */
void EntityManager::StoreEntity(Worlds::Room& room, Entity& entity)
{
    // Storing pointers is safe because both rooms and entities are tied to the world's lifespan
    m_EntityStorage[&room].emplace_back(&entity);
}

/**
 * @brief Try to move the player in the given direction
 * This counts as a player action and results in entities cycling if successful.
 *
 * @param dir direction
 * @return true if move successful
 */
bool EntityManager::TryMovePlayerEntity(Direction dir)
{
    if (CanCharacterMove(m_Player, dir))
    {
        VacateEntityFieldInRoom(m_Player, m_WorldManager.GetCurrentRoom());
        m_Player.Move(dir);
        PlaceEntityInRoom(m_Player, m_WorldManager.GetCurrentRoom());
        CycleEntitiesInCurrentRoom();
        return true;
    }
    else if (m_WorldManager.GetCurrentRoom().IsPositionAtRoomEdge(m_Player.GetCoords(), dir))
    {
        Direction nextRoomEntranceDir = dir.Opposite();
        VacateEntityFieldInRoom(m_Player, m_WorldManager.GetCurrentRoom());
        Worlds::Room& nextRoom = m_WorldManager.SwitchCurrentRoom(dir);
        Coords newCoords = nextRoom
                               .TryGetEntrance(nextRoomEntranceDir)
                               ->GetCoords();
        m_Player.SetCoords(newCoords);
        m_Player.SetLastMoveDirection(dir);
        PlaceEntityInRoom(m_Player, nextRoom);
        CycleEntitiesInCurrentRoom();
        return true;
    }

    m_Player.SetLastMoveDirection(dir);
    return false;
}

/**
 * @brief Get the entity the character is touching
 * This is any entity next to the character in the direction of its last move.
 *
 * @param character character
 * @return const Entity* entity being approached
 */
const Entity* EntityManager::GetApproachedEntity(const Character& approachingCharacter) const
{
    const Worlds::Field* approachedField = GetFieldNextToEntity(approachingCharacter, approachingCharacter.GetLastMoveDirection());
    return approachedField != nullptr ? approachedField->TryGetForegroundEntity() : nullptr;
}

/**
 * @brief Check if the character can move in the given direction
 *
 * @param character character
 * @param dir direction
 * @return true if can move
 */
bool EntityManager::CanCharacterMove(const Character& character, Direction dir) const
{
    if (dir == Direction::None()) return true;

    const Worlds::Field* targetField = GetFieldNextToEntity(character, dir);
    if (targetField != nullptr && targetField->TryGetForegroundEntity() == nullptr)
    {
        return true;
    }

    return false;
}

/**
 * @brief Get an array of fields surrounding the entity
 *
 * @param entity entity
 * @return const std::array<const Worlds::Field*, 4> surrounding fields
 */
const std::array<const Worlds::Field*, 4> EntityManager::GetFieldsNextToEntity(
    const Entity& entity) const
{
    Coords coords = entity.GetCoords();
    const Worlds::Room& currentRoom = m_WorldManager.GetCurrentRoom();
    std::array<const Worlds::Field*, 4> fields;
    for (const auto& dir : Direction::All())
    {
        fields[dir.ToInt()] = !currentRoom.IsPositionAtRoomEdge(coords, dir)
                                  ? &currentRoom.GetFieldAt(coords.GetAdjacent(dir))
                                  : nullptr;
    }
    return fields;
}

/**
 * @brief Get the field next to the entity in the given direction
 *
 * @param entity entity
 * @param direction direction
 * @return const Worlds::Field* neighboring field
 */
const Worlds::Field* EntityManager::GetFieldNextToEntity(
    const Entity& entity,
    Direction direction) const
{
    return direction != Direction::None()
               ? GetFieldsNextToEntity(entity)[direction.ToInt()]
               : nullptr;
}

/**
 * @brief Perform behavior for all entities in the given room
 *
 * @param room room
 */
void EntityManager::CycleEntitiesInRoom(Worlds::Room& room)
{
    for (auto& entity : m_EntityStorage[&room])
    {
        VacateEntityFieldInRoom(*entity, room);
        //TODO: Implement NPC behavior
        //TODO: Resolve movement conflicts between player and NPCs or two NPCs
        PlaceEntityInRoom(*entity, room);
    }
}

/**
 * @brief Place the entity in its position in the given room
 *
 * @param entity entity
 * @param room room
 */
void EntityManager::PlaceEntityInRoom(Entity& entity, Worlds::Room& room)
{
    auto& field = room.GetFieldAt(entity.GetCoords());
    field.PlaceEntity(entity);
}

/**
 * @brief Vacate the field occupied by the entity in the given room
 *
 * @param entity entity
 * @param room room
 */
void EntityManager::VacateEntityFieldInRoom(Entity& entity, Worlds::Room& room)
{
    auto& field = room.GetFieldAt(entity.GetCoords());
    entity.IsBlocking() ? field.VacateForeground() : field.VacateBackground();
}

} /* namespace Entities */

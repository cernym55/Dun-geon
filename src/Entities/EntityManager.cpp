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

void EntityManager::CycleEntitiesInCurrentRoom()
{
    CycleEntitiesInRoom(m_WorldManager.GetCurrentRoom());
}

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
    else if (IsCharacterAboutToLeaveRoom(m_Player, dir))
    {
        Direction nextRoomEntranceDir = dir.Opposite();
        VacateEntityFieldInRoom(m_Player, m_WorldManager.GetCurrentRoom());
        Worlds::Room& nextRoom = m_WorldManager.SwitchCurrentRoom(dir);
        Coords newCoords = nextRoom
                               .GetEntrance(nextRoomEntranceDir)
                               .GetCoords();
        m_Player.SetCoords(newCoords);
        m_Player.SetLastMoveDirection(dir);
        PlaceEntityInRoom(m_Player, nextRoom);
        CycleEntitiesInCurrentRoom();
        return true;
    }

    m_Player.SetLastMoveDirection(dir);
    return false;
}

const Entity* EntityManager::GetApproachedEntity(const Character& approachingCharacter) const
{
    const Worlds::Field* approachedField = GetFieldNextToEntity(approachingCharacter, approachingCharacter.GetLastMoveDirection());
    return approachedField != nullptr ? approachedField->GetForegroundEntity() : nullptr;
}

bool EntityManager::IsCharacterAboutToLeaveRoom(const Character& character,
                                                Direction moveDirection) const
{
    switch (moveDirection())
    {
    case Direction::Value::Up:
        return character.GetCoords().GetY() == 0;
    case Direction::Value::Right:
        return character.GetCoords().GetX() == m_WorldManager.GetCurrentRoom().GetWidth() - 1;
    case Direction::Value::Down:
        return character.GetCoords().GetY() == m_WorldManager.GetCurrentRoom().GetHeight() - 1;
    case Direction::Value::Left:
        return character.GetCoords().GetX() == 0;
    default:
        return false;
    }
}

bool EntityManager::CanCharacterMove(const Character& character, Direction dir) const
{
    if (dir == Direction::None()) return true;

    const Worlds::Field* targetField = GetFieldNextToEntity(character, dir);
    if (targetField != nullptr && targetField->GetForegroundEntity() == nullptr)
    {
        return true;
    }

    return false;
}

const std::array<const Worlds::Field*, 4> EntityManager::GetFieldsNextToEntity(
    const Entity& entity) const
{
    Coords coords = entity.GetCoords();
    const Worlds::Room& currentRoom = m_WorldManager.GetCurrentRoom();
    return { { coords.GetY() != 0 ? &currentRoom.GetFieldAt(coords.GetAdjacent(Direction::Up())) : nullptr,
               coords.GetX() != currentRoom.GetWidth() - 1 ? &currentRoom.GetFieldAt(coords.GetAdjacent(Direction::Right())) : nullptr,
               coords.GetY() != currentRoom.GetHeight() - 1 ? &currentRoom.GetFieldAt(coords.GetAdjacent(Direction::Down())) : nullptr,
               coords.GetX() != 0 ? &currentRoom.GetFieldAt(coords.GetAdjacent(Direction::Left())) : nullptr } };
}

const Worlds::Field* EntityManager::GetFieldNextToEntity(
    const Entity& entity,
    Direction direction) const
{
    return direction != Direction::None()
               ? GetFieldsNextToEntity(entity)[static_cast<int>(direction())]
               : nullptr;
}

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

void EntityManager::PlaceEntityInRoom(Entity& entity, Worlds::Room& room)
{
    auto& field = room.GetFieldAt(entity.GetCoords());
    field.PlaceEntity(entity);
}

void EntityManager::VacateEntityFieldInRoom(Entity& entity, Worlds::Room& room)
{
    auto& field = room.GetFieldAt(entity.GetCoords());
    entity.IsBlocking() ? field.VacateForeground() : field.VacateBackground();
}

} /* namespace Entities */
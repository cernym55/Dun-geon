#include "Controller.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Direction.h"
#include "Worlds/Field.h"
#include "Worlds/WorldManager.h"

namespace Player
{

Controller::Controller(Entities::EntityManager& entityManager,
                       Worlds::WorldManager& worldManager,
                       Entities::Player& playerEntity)
    : m_EntityManager(entityManager),
      m_WorldManager(worldManager),
      m_PlayerEntity(playerEntity)
{
}

bool Controller::TryMovePlayer(Direction dir)
{
    return m_EntityManager.TryMovePlayerEntity(dir);
}

bool Controller::TryMovePlayerDiagonally(Direction first, Direction second)
{
    auto playerCoords = m_PlayerEntity.GetCoords();
    auto& room = m_WorldManager.GetCurrentRoom();
    auto firstNeighbor = room.IsPositionAtRoomEdge(playerCoords, first)
            ? nullptr
            : &room.GetFieldAt(playerCoords.GetAdjacent(first));
    auto secondNeighbor = room.IsPositionAtRoomEdge(playerCoords, second)
            ? nullptr
            : &room.GetFieldAt(playerCoords.GetAdjacent(second));
    auto target = firstNeighbor == nullptr || secondNeighbor == nullptr
            ? nullptr
            : &room.GetFieldAt(playerCoords.GetAdjacent(first).GetAdjacent(second));
    if (target != nullptr && target->TryGetForegroundEntity() != nullptr)
        return false;

    if (firstNeighbor == nullptr || firstNeighbor->TryGetForegroundEntity() == nullptr)
        return m_EntityManager.TryMovePlayerEntity(first) && m_EntityManager.TryMovePlayerEntity(second);
    else if (secondNeighbor == nullptr || secondNeighbor->TryGetForegroundEntity() == nullptr)
        return m_EntityManager.TryMovePlayerEntity(second) && m_EntityManager.TryMovePlayerEntity(first);
    else
        return false;
}

} /* namespace Player */

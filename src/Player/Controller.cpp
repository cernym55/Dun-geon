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
    return m_EntityManager.TryMovePlayer(dir);
}

bool Controller::TryMovePlayerDiagonally(Direction first, Direction second)
{
    auto playerCoords = m_PlayerEntity.GetCoords();
    auto& room = m_WorldManager.CurrentRoom();
    auto firstNeighbor = room.IsAtRoomEdge(playerCoords, first)
            ? nullptr
            : &room.FieldAt(playerCoords.Adjacent(first));
    auto secondNeighbor = room.IsAtRoomEdge(playerCoords, second)
            ? nullptr
            : &room.FieldAt(playerCoords.Adjacent(second));
    auto target = firstNeighbor == nullptr || secondNeighbor == nullptr
            ? nullptr
            : &room.FieldAt(playerCoords.Adjacent(first).Adjacent(second));
    if (target != nullptr && target->ForegroundEntity() != nullptr)
        return false;

    if (firstNeighbor == nullptr || firstNeighbor->ForegroundEntity() == nullptr)
        return m_EntityManager.TryMovePlayer(first) && m_EntityManager.TryMovePlayer(second);
    else if (secondNeighbor == nullptr || secondNeighbor->ForegroundEntity() == nullptr)
        return m_EntityManager.TryMovePlayer(second) && m_EntityManager.TryMovePlayer(first);
    else
        return false;
}

} /* namespace Player */

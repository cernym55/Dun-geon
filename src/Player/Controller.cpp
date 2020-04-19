#include "Controller.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Direction.h"
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
    if (m_PlayerEntity.CanMove(dir))
    {
        m_PlayerEntity.move(dir);
        return true;
    }
    else if (IsPlayerAboutToLeaveRoom(dir))
    {
        Direction nextRoomEntranceDir = dir.Opposite();
        m_WorldManager.SwitchCurrentRoom(dir);
        m_PlayerEntity.SwitchCurrentRoom(dir);
        Coords newCoords = const_cast<const Worlds::WorldManager&>(m_WorldManager)
                               .GetCurrentRoom()
                               .GetEntrance(nextRoomEntranceDir)
                               .GetCoords();
        m_PlayerEntity.SetCoords(newCoords);
        return true;
    }

    return false;
}

bool Controller::IsPlayerAboutToLeaveRoom(Direction moveDirection) const
{
    switch (moveDirection())
    {
    case Direction::Value::Up:
        return m_PlayerEntity.GetCoords().GetY() == 0;
    case Direction::Value::Right:
        return m_PlayerEntity.GetCoords().GetX() == m_PlayerEntity.getCurrentRoom()->GetWidth() - 1;
    case Direction::Value::Down:
        return m_PlayerEntity.GetCoords().GetY() == m_PlayerEntity.getCurrentRoom()->GetHeight() - 1;
    case Direction::Value::Left:
        return m_PlayerEntity.GetCoords().GetX() == 0;
    default:
        return false;
    }
}

} /* namespace Player */
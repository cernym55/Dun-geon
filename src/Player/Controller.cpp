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
    return m_EntityManager.TryMovePlayerEntity(dir);
}

} /* namespace Player */
#include "Battle/Battle.h"
#include "Controller.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Direction.h"
#include "Worlds/Field.h"
#include "Worlds/WorldManager.h"
#include "UI/BattleScreen.h"
#include "UI/Screen.h"

namespace Player
{

Controller::Controller(Entities::EntityManager& entityManager,
                       Worlds::WorldManager& worldManager,
                       Entities::Player& playerEntity,
                       UI::Screen& screen)
    : m_EntityManager(entityManager),
      m_WorldManager(worldManager),
      m_PlayerEntity(playerEntity),
      m_Screen(screen)
{
}

bool Controller::TryMovePlayer(Direction dir)
{
    return m_EntityManager.TryMovePlayer(dir);
}

bool Controller::TryMovePlayerDiagonally(Direction first, Direction second)
{
    auto playerCoords = m_EntityManager.CoordsOf(m_PlayerEntity);
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

bool Controller::TryFight(Direction dir)
{
    auto approaching = m_EntityManager.Approaching(m_PlayerEntity, dir);
    if (approaching == nullptr || !approaching->Fightable()) return false;

    // TODO: remove the cast
    Entities::Character& targetedCharacter = dynamic_cast<Entities::Character&>(*approaching);
    Battle::Battle battle(m_PlayerEntity, targetedCharacter);
    m_Screen.OpenBattleScreen(battle);
    // do battle
    m_Screen.CloseSubscreen();

    return true;
}

} /* namespace Player */

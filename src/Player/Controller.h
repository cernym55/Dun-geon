#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Direction.h"
#include "Worlds/WorldManager.h"

namespace UI { class Screen; }

namespace Player
{

/**
 * @brief Manipulates the Player objects and performs actions on their behalf
 */
class Controller
{
public:
    /**
     * @brief Constructor
     * 
     * @param entityManager entity manager
     * @param worldManager world manager
     * @param playerEntity player entity
     * @param screen screen
     */
    Controller(Entities::EntityManager& entityManager,
               Worlds::WorldManager& worldManager,
               Entities::Player& playerEntity,
               UI::Screen& screen);

    /**
     * @brief Try to move the player in the given direction
     * 
     * @param dir direction
     * @return true if move successfully performed
     */
    bool TryMovePlayer(Direction dir);

    /**
     * @brief Try to move the player diagonally (two successive moves)
     *
     * @param first first direction
     * @param second second direction
     * @return true if both moves successfully performed
     */
    bool TryMovePlayerDiagonally(Direction first, Direction second);

    /**
     * @brief Try to fight something in the given direction
     * 
     * @param dir direction
     * @return true if there is something to fight, false otherwise
     */
    bool TryFight(Direction dir);

    /**
     * @brief Turn the player in the given direction
     * 
     * @param dir direction
     */
    void TurnPlayer(Direction dir);

private:
    Entities::EntityManager& m_EntityManager;
    Worlds::WorldManager& m_WorldManager;
    Entities::Player& m_PlayerEntity;
    UI::Screen& m_Screen;
};

} /* namespace Player */

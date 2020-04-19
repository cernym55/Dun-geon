#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Direction.h"
#include "Worlds/WorldManager.h"

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
     */
    Controller(Entities::EntityManager& entityManager,
               Worlds::WorldManager& worldManager,
               Entities::Player& playerEntity);

    /**
     * @brief Delete copy constructor
     */
    Controller(const Controller&) = delete;

    /**
     * @brief Default destructor
     */
    ~Controller() = default;

    /**
     * @brief Try to move the player in the given direction.
     * 
     * @param dir direction
     * @return true if move successfully performed
     */
    bool TryMovePlayer(Direction dir);

private:
    /**
     * @brief Check if the player is going to leave the room if they move in the given direction.
     * 
     * @param moveDirection direction of the move
     * @return true if player will leave the room
     */
    bool IsPlayerAboutToLeaveRoom(Direction moveDirection) const;

    Entities::EntityManager& m_EntityManager;
    Worlds::WorldManager& m_WorldManager;
    Entities::Player& m_PlayerEntity;
};

} /* namespace Player */
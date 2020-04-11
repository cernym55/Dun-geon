#pragma once

#include "entities/player.h"
#include "world.h"
#include <memory>
#include <vector>

namespace Worlds
{

/**
 * @brief Class for managing Worlds and their creation
 */
class WorldManager
{
public:
    /**
     * @brief Constructor
     * 
     * @param player player
     */
    WorldManager(Entities::Player& player);

    /**
     * @brief Delete copy constructor
     */
    WorldManager(const WorldManager&) = delete;

    /**
     * @brief Default destructor
     */
    ~WorldManager() = default;

    /**
     * @brief Get the currently active world
     * 
     * @return World& current world
     */
    World& GetCurrentWorld();

    /**
     * @brief Get the player position
     * 
     * @return std::pair<int, int> position (x, y)
     */
    std::pair<int, int> GetPlayerPosition() const;

private:
    /**
     * @brief Get the Player object
     * 
     * @return const Entities::Player& player
     */
    const Entities::Player& GetPlayer() const;

    const Entities::Player& m_Player;
    std::vector<std::unique_ptr<World>> m_Worlds;
    int m_NextWorldNum;
};

} /* namespace Worlds */
#pragma once

#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include <memory>
#include <vector>

namespace Worlds
{

class World;
class Room;

/**
 * @brief Class for managing Worlds and their creation
 */
class WorldManager
{
public:
    /**
     * @brief Constructor
     */
    WorldManager();

    /**
     * @brief Delete copy constructor
     */
    WorldManager(const WorldManager&) = delete;

    /**
     * @brief Default destructor
     */
    ~WorldManager() = default;

    /**
     * @brief Get the world where the player is present
     * 
     * @return const World& current world
     */
    const World& GetCurrentWorld() const;

    /**
     * @brief Get the room where the player is present
     * 
     * @return Room& current room
     */
    Room& GetCurrentRoom();

    /**
     * @brief Get the room where the player is present
     * 
     * @return const Room& current room
     */
    const Room& GetCurrentRoom() const;

    /**
     * @brief Check if the given room is the current room
     * 
     * @param room room
     * @return true if current room
     */
    bool IsCurrentRoom(const Room& room) const;

    /**
     * @brief Transition to the neighboring room in the given direction
     * 
     * @param dir direction
     * @return Room& new current room
     */
    Room& SwitchCurrentRoom(Direction dir);

private:
    /**
     * @brief Get the world where the player is present
     * 
     * @return World& current world
     */
    World& GetCurrentWorld();

    /**
     * @brief Create a new world
     * 
     * @return World& new world
     */
    World& CreateWorld();

    /**
     * @brief Return the next world number and increment the counter
     * 
     * @return int next world number
     */
    int PopNextWorldNumber();

    std::vector<std::unique_ptr<World>> m_Worlds;
    int m_NextWorldNumber;
    World* m_CurrentWorld;
    Coords m_CurrentRoomCoords;
};

} /* namespace Worlds */
#pragma once

#include "Misc/Coords.h"
#include "WorldManager.h"
#include <memory>
#include <vector>

namespace Worlds
{

class Room;

/**
 * @brief A world represents a game level and is comprised of rooms
 */
class World
{
public:
    /**
     * @brief Maximum span/width/height of a world grid
     */
    constexpr static const size_t MaxSpan = 21;

    /**
     * @brief Center position index on the world grid
     */
    constexpr static const size_t CenterPos = MaxSpan / 2;

    /**
     * @brief Constructor
     * 
     * @param worldManager World manager
     * @param worldNumber World number
     */
    World(WorldManager& worldManager, int worldNumber);

    /**
     * @brief Delete copy constructor
     */
    World(const World&) = delete;

    /**
     * @brief Default destructor
     */
    ~World() = default;

    /**
     * @brief Get the world number
     * 
     * @return int world number
     */
    int GetWorldNumber() const;

    /**
     * @brief Get the room at the specified position of the world grid
     * 
     * @param coords coordinates
     * @return Room& room at the given grid position
     */
    Room& GetRoomAt(Coords coords);

    /**
     * @brief Get the room at the specified position
     * 
     * @param coords coordinates
     * @return const Room& room at the given position
     */
    const Room& GetRoomAt(Coords coords) const;

    /**
     * @brief Get the starting room of this world
     * 
     * @return Room& starting room
     */
    Room& GetStartingRoom();

    /**
     * @brief Get the starting room of this world
     * 
     * @return const Room& starting room
     */
    const Room& GetStartingRoom() const;

    /**
     * @brief Create a room at the specified position
     * 
     * @param coords coordinates
     * @return Room& new room
     */
    Room& CreateRoom(Coords coords);

private:
    /**
     * @brief Return the next room number and increment the counter
     * 
     * @return int next room number
     */
    int PopNextRoomNumber();

    WorldManager& m_WorldManager;
    int m_WorldNumber;
    int m_NextRoomNumber;
    std::vector<std::vector<std::unique_ptr<Room>>> m_Rooms;
};

} /* namespace Worlds */
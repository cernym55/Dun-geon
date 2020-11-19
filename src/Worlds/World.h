#pragma once

#include "Generation/RoomGenerator.h"
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
    constexpr static const Coords::Scalar MaximumSpan = 21;

    /**
     * @brief Center position index on the world grid
     */
    constexpr static const Coords::Scalar CenterPos = MaximumSpan / 2;

    /**
     * @brief Constructor
     * 
     * @param worldManager World manager
     * @param worldNumber World number
     */
    World(WorldManager& worldManager, int worldNumber);

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
    Room& RoomAt(Coords coords);

    /**
     * @brief Get the room at the specified position
     * 
     * @param coords coordinates
     * @return const Room& room at the given position
     */
    const Room& RoomAt(Coords coords) const;

    /**
     * @brief Check if the coords are at the world grid edge in the given direction
     * 
     * @param coords coordinates
     * @param dir direction
     * @return true if at world grid edge
     */
    bool IsAtWorldGridEdge(Coords coords, Direction dir) const;

    /**
     * @brief Get the starting room of this world
     * 
     * @return Room& starting room
     */
    Room& StartingRoom();

    /**
     * @brief Get the starting room of this world
     * 
     * @return const Room& starting room
     */
    const Room& StartingRoom() const;

    /**
     * @brief Create a room at the specified position
     * 
     * @param coords coordinates
     * @return Room& new room
     */
    Room& CreateRoom(Coords coords);

    /**
     * @brief Check if a room exists at the given coordinates
     * 
     * @param coords coordinates
     * @return true if a room exists at the given coordinates
     */
    bool RoomExists(Coords coords) const;

private:
    WorldManager& m_WorldManager;
    Generation::RoomGenerator m_RoomGenerator;
    int m_WorldNumber;
    int m_NextRoomNumber;
    std::vector<std::vector<std::unique_ptr<Room>>> m_Rooms;

    /**
     * @brief Return the next room number and increment the counter
     * 
     * @return int next room number
     */
    int PopRoomNumber();

    /**
     * @brief Create the starting room for this world
     */
    void CreateStartingRoom();
};

} /* namespace Worlds */

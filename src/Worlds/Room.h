#pragma once

#include "Entities/Entity.h"
#include "Field.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "World.h"
#include "WorldManager.h"
#include <array>
#include <vector>

namespace Worlds
{

enum class Layout
{
    RandLayout = -1,
    Box = 0,
};

/**
 * @brief A room is the smallest map unit that the player can move around in
 */
class Room
{
public:
    /**
     * @brief Constructor
     * 
     * @param worldManager world manager
     * @param world world
     * @param roomNumber room number
     * @param coords coordinates
     */
    Room(WorldManager& worldManager, World& world, int roomNumber, Coords coords);

    /**
     * @brief Delete copy constructor
     */
    Room(const Room&) = delete;

    /**
     * @brief Default destructor
     */
    ~Room() = default;

    /**
     * @brief Get the coordinates
     * 
     * @return Coords coordinates
     */
    Coords GetCoords() const;

    /**
     * @brief Get the world this room belongs to
     * 
     * @return const World& world
     */
    const World& GetWorld() const;

    /**
     * @brief Get the width
     * 
     * @return size_t width
     */
    size_t GetWidth() const;

    /**
     * @brief Get the height
     * 
     * @return size_t height
     */
    size_t GetHeight() const;

    /**
     * @brief Get the room number
     * 
     * @return int room number
     */
    int GetRoomNumber() const;

    /**
     * @brief Check if the room is at the edge of the world grid in the given direction
     * 
     * @param dir direction
     * @return true if at the edge
     */
    bool IsAtWorldGridEdge(Direction dir) const;

    /**
     * @brief Get the entrance field in the given direction
     * 
     * @param dir direction
     * @return const Field& entrance field
     */
    const Field* TryGetEntrance(Direction dir) const;

    /**
     * @brief Check if the room has a neighboring room in the given direction
     * 
     * @param dir direction
     * @return true if a neighboring room exists in the given direction
     */
    bool HasNeighbor(Direction dir) const;

    /**
     * @brief Get the neighboring room in the given direction
     * 
     * @param dir direction
     * @return const Room& neighboring room
     */
    const Room& GetNeighbor(Direction dir) const;

    /**
     * @brief Get the field at the specified coords
     * 
     * @param coords coordinates
     * @return Field& target field
     */
    Field& GetFieldAt(Coords coords);

    /**
     * @brief Get the field at the specified coords
     * 
     * @param coords coordinates
     * @return const Field& target field
     */
    const Field& GetFieldAt(Coords coords) const;

    /**
     * @brief Generate the layout of the room
     * 
     * @param layout layout type
     * @param forcedEntrances whether or not to force generating an entrance if possible (for each side)
     */
    void Generate(Layout layout, std::array<bool, 4> forceEntrances = { false, false, false, false });

protected:
    WorldManager& m_WorldManager;
    World& m_World;
    int m_RoomNumber;
    Coords m_Coords;
    size_t m_Width;
    size_t m_Height;
    std::array<Field*, 4> m_Entrances;
    std::vector<std::vector<Field>> m_Fields;
};

} /* namespace Worlds */
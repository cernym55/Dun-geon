#pragma once

#include "Entities/Entity.h"
#include "Field.h"
#include "Generation/RoomLayout.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "UI/CameraStyle.h"
#include "World.h"
#include "WorldManager.h"
#include <array>
#include <vector>

namespace Worlds
{

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
     * @param layout layout
     * @param roomNumber room number
     * @param coords coordinates
     */
    Room(WorldManager& worldManager,
         World& world,
         const Generation::RoomLayout& layout,
         int roomNumber,
         Coords coords);

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
     * @return Coords::Scalar width
     */
    Coords::Scalar GetWidth() const;

    /**
     * @brief Get the height
     * 
     * @return Coords::Scalar height
     */
    Coords::Scalar GetHeight() const;

    /**
     * @brief Get the camera style
     * 
     * @return CameraStyle camera style
     */
    UI::CameraStyle GetCameraStyle() const;

    /**
     * @brief Get the vision radius
     * Value 0 is considered a lit room.
     * 
     * @return int vision radius
     */
    int GetVisionRadius() const;

    /**
     * @brief Get the room number
     * 
     * @return int room number
     */
    int GetRoomNumber() const;

    /**
     * @brief Get the entrance field in the given direction
     * 
     * @param dir direction
     * @return const Field& entrance field
     */
    const Field* Entrance(Direction dir) const;

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
    const Room& Neighbor(Direction dir) const;

    /**
     * @brief Get the field at the specified coords
     * 
     * @param coords coordinates
     * @return Field& target field
     */
    Field& FieldAt(Coords coords);

    /**
     * @brief Get the field at the specified coords
     * 
     * @param coords coordinates
     * @return const Field& target field
     */
    const Field& FieldAt(Coords coords) const;

    /**
     * @brief Check if the coords are at the edge of this room
     * 
     * @param coords coords
     * @param dir edge direction
     * @return true if at the edge
     */
    bool IsAtRoomEdge(Coords coords, Direction dir) const;

protected:
    WorldManager& m_WorldManager;
    World& m_World;
    int m_RoomNumber;
    Coords m_Coords;
    Coords::Scalar m_Width;
    Coords::Scalar m_Height;
    std::array<Field*, 4> m_Entrances;
    std::vector<std::vector<Field>> m_Fields;
    UI::CameraStyle m_CameraStyle;
    int m_VisionRadius;
};

} /* namespace Worlds */

#include "Room.h"
#include "Entities/Player.h"
#include "Entities/StaticEntities.h"
#include "Field.h"
#include "Generation/RoomLayout.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Misc/RNG.h"
#include "UI/CameraStyle.h"
#include "World.h"
#include "WorldManager.h"
#include <array>
#include <sstream>

namespace Worlds
{

Room::Room(WorldManager& worldManager,
           World& world,
           const Generation::RoomLayout& layout,
           int roomNumber,
           Coords coords)
    : m_WorldManager(worldManager),
      m_World(world),
      m_RoomNumber(roomNumber),
      m_Coords(coords),
      m_CameraStyle(layout.GetCameraStyle()),
      m_VisionRadius(layout.GetVisionRadius())
{
    layout.WriteToFields(m_Fields);
    m_Width = m_Fields.size();
    m_Height = m_Fields[0].size();
    const auto& entrances = layout.GetEntrances();
    for (const auto& dir : Direction::All())
    {
        m_Entrances[dir.ToInt()] = entrances.count(dir) > 0 ? &GetFieldAt(entrances.at(dir)) : nullptr;
    }
}

Coords Room::GetCoords() const
{
    return m_Coords;
}

const World& Room::GetWorld() const
{
    return m_World;
}

size_t Room::GetWidth() const
{
    return m_Width;
}

size_t Room::GetHeight() const
{
    return m_Height;
}

UI::CameraStyle Room::GetCameraStyle() const
{
    return m_CameraStyle;
}

int Room::GetVisionRadius() const
{
    return m_VisionRadius;
}

int Room::GetRoomNumber() const
{
    return m_RoomNumber;
}

const Field* Room::TryGetEntrance(Direction dir) const
{
    return m_Entrances[(int)dir()];
}

bool Room::HasNeighbor(Direction dir) const
{
    if (m_World.IsPositionAtWorldGridEdge(m_Coords, dir)) return false;

    return m_World.RoomExistsAt(m_Coords.GetAdjacent(dir));
}

const Room& Room::GetNeighbor(Direction dir) const
{
    if (HasNeighbor(dir))
    {
        return m_World.GetRoomAt(m_Coords.GetAdjacent(dir));
    }

    std::ostringstream errorMessage;
    errorMessage << "Requested nonexistent neighboring room in direction "
                 << dir;
    throw std::invalid_argument(errorMessage.str());
}

Field& Room::GetFieldAt(Coords coords)
{
    if (coords.GetX() >= m_Width || coords.GetY() >= m_Height)
    {
        std::ostringstream errorMessage;
        errorMessage << "Room field coords out of bounds: "
                     << coords;
        throw std::out_of_range(errorMessage.str());
    }
    else
    {
        return m_Fields[coords.GetX()][coords.GetY()];
    }
}

const Field& Room::GetFieldAt(Coords coords) const
{
    if (coords.GetX() >= m_Width || coords.GetY() >= m_Height)
    {
        std::ostringstream errorMessage;
        errorMessage << "Room field coords out of bounds: "
                     << coords;
        throw std::out_of_range(errorMessage.str());
    }
    else
    {
        return m_Fields[coords.GetX()][coords.GetY()];
    }
}

bool Room::IsPositionAtRoomEdge(Coords coords, Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return coords.GetY() == 0;
    case Direction::Value::Right:
        return coords.GetX() == m_Width - 1;
    case Direction::Value::Down:
        return coords.GetY() == m_Height - 1;
    case Direction::Value::Left:
        return coords.GetX() == 0;
    default:
        return false;
    }
}

} /* namespace Worlds */
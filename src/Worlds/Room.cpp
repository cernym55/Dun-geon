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
      m_VisionRadius(layout.GetVisionRadius()),
      m_NPCSpawnChance(layout.GetNPCSpawnChance())
{
    m_AccessibleFieldCount = layout.WriteToFields(m_Fields);
    m_Width = m_Fields.size();
    m_Height = m_Fields[0].size();
    const auto& entrances = layout.GetEntrances();
    for (const auto& dir : Direction::All)
    {
        m_Entrances[dir.ToInt()] = entrances.count(dir) > 0 ? &FieldAt(entrances.at(dir)) : nullptr;
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

Coords::Scalar Room::GetWidth() const
{
    return m_Width;
}

Coords::Scalar Room::GetHeight() const
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

int Room::AccessibleFieldCount() const
{
    return m_AccessibleFieldCount;
}

const Field* Room::Entrance(Direction dir) const
{
    return m_Entrances[static_cast<size_t>(dir())];
}

bool Room::HasNeighbor(Direction dir) const
{
    if (m_World.IsAtWorldGridEdge(m_Coords, dir)) return false;

    return m_World.RoomExists(m_Coords.Adjacent(dir));
}

const Room& Room::Neighbor(Direction dir) const
{
    if (HasNeighbor(dir))
    {
        return m_World.RoomAt(m_Coords.Adjacent(dir));
    }

    std::ostringstream errorMessage;
    errorMessage << "Requested nonexistent neighboring room in direction "
                 << dir;
    throw std::invalid_argument(errorMessage.str());
}

Field& Room::FieldAt(Coords coords)
{
    if (coords.X >= m_Width || coords.Y >= m_Height ||
        coords.X < 0 || coords.Y < 0)
    {
        std::ostringstream errorMessage;
        errorMessage << "Room field coords out of bounds: "
                     << coords;
        throw std::out_of_range(errorMessage.str());
    }
    else
    {
        return m_Fields[coords.X][coords.Y];
    }
}

const Field& Room::FieldAt(Coords coords) const
{
    if (coords.X >= m_Width || coords.Y >= m_Height ||
        coords.X < 0 || coords.Y < 0)
    {
        std::ostringstream errorMessage;
        errorMessage << "Room field coords out of bounds: "
                     << coords;
        throw std::out_of_range(errorMessage.str());
    }
    else
    {
        return m_Fields[coords.X][coords.Y];
    }
}

bool Room::IsAtRoomEdge(Coords coords, Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return coords.Y == 0;
    case Direction::Value::Right:
        return coords.X == m_Width - 1;
    case Direction::Value::Down:
        return coords.Y == m_Height - 1;
    case Direction::Value::Left:
        return coords.X == 0;
    default:
        return false;
    }
}

double Room::GetNPCSpawnChance() const
{
    return m_NPCSpawnChance;
}

} /* namespace Worlds */

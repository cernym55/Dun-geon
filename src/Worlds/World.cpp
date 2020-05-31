#include "World.h"
#include "Generation/RoomGenerator.h"
#include "Generation/RoomLayout.h"
#include "Misc/Coords.h"
#include "Misc/Exceptions.h"
#include "Misc/RNG.h"
#include "Room.h"
#include "WorldManager.h"
#include <array>
#include <exception>
#include <memory>
#include <sstream>

namespace Worlds
{

World::World(WorldManager& worldManager, int worldNumber)
    : m_WorldManager(worldManager),
      m_RoomGenerator(*this),
      m_WorldNumber(worldNumber),
      m_NextRoomNumber(1)
{
    m_Rooms.resize(MaximumSpan);
    for (auto& vec : m_Rooms)
        vec.resize(MaximumSpan);
    CreateStartingRoom();
}

int World::GetWorldNumber() const
{
    return m_WorldNumber;
}

Room& World::RoomAt(Coords coords)
{
    if (coords.X >= MaximumSpan || coords.Y >= MaximumSpan ||
        coords.X < 0 || coords.Y < 0)
    {
        std::ostringstream errorMessage;
        errorMessage << "World grid position out of bounds: "
                     << coords;
        throw InvalidPositionException(errorMessage.str());
    }

    if (m_Rooms[coords.X][coords.Y] == nullptr)
    {
        std::ostringstream errorMessage;
        errorMessage << "Room "
                     << coords
                     << " of world "
                     << m_WorldNumber
                     << " is uninitialized";
        throw std::invalid_argument(errorMessage.str());
    }

    return *m_Rooms[coords.X][coords.Y];
}

const Room& World::RoomAt(Coords coords) const
{
    if (coords.X >= MaximumSpan || coords.Y >= MaximumSpan ||
        coords.X < 0 || coords.Y < 0)
    {
        std::ostringstream errorMessage;
        errorMessage << "World grid position out of bounds: "
                     << coords;
        throw InvalidPositionException(errorMessage.str());
    }

    if (m_Rooms[coords.X][coords.Y] == nullptr)
    {
        std::ostringstream errorMessage;
        errorMessage << "Room "
                     << coords
                     << " of world "
                     << m_WorldNumber
                     << " is uninitialized";
        throw std::invalid_argument(errorMessage.str());
    }

    return *m_Rooms[coords.X][coords.Y];
}

bool World::IsAtWorldGridEdge(Coords coords, Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return coords.Y == 0;
    case Direction::Value::Right:
        return coords.X == MaximumSpan - 1;
    case Direction::Value::Down:
        return coords.Y == MaximumSpan - 1;
    case Direction::Value::Left:
        return coords.X == 0;
    default:
        return false;
    }
}

Room& World::StartingRoom()
{
    return RoomAt({ CenterPos, CenterPos });
}

const Room& World::StartingRoom() const
{
    return RoomAt({ CenterPos, CenterPos });
}

Room& World::CreateRoom(Coords coords)
{
    if (RoomExists(coords))
    {
        std::ostringstream errorMessage;
        errorMessage << "Attempted to create room overlapping with existing room at "
                     << coords;
        throw std::invalid_argument(errorMessage.str());
    }

    auto layout = m_RoomGenerator.CreateLayout(coords);
    m_Rooms[coords.X][coords.Y] = std::make_unique<Room>(
        m_WorldManager,
        *this,
        *layout,
        PopRoomNumber(),
        coords);

    return *m_Rooms[coords.X][coords.Y];
}

bool World::RoomExists(Coords coords) const
{
    if (coords.X >= MaximumSpan || coords.Y >= MaximumSpan ||
        coords.X < 0 || coords.Y < 0)
    {
        return false;
    }
    return m_Rooms[coords.X][coords.Y] != nullptr;
}

int World::PopRoomNumber()
{
    return m_NextRoomNumber++;
}

void World::CreateStartingRoom()
{
    Coords coords = { CenterPos, CenterPos };
    auto layout = m_RoomGenerator.CreateLayout(Generation::RoomLayout::Type::Box, coords);
    m_Rooms[CenterPos][CenterPos] = std::make_unique<Room>(
        m_WorldManager,
        *this,
        *layout,
        PopRoomNumber(),
        coords);
}

} /* namespace Worlds */

#include "World.h"
#include "Misc/Coords.h"
#include "Misc/RNG.h"
#include "Generation/RoomGenerator.h"
#include "Generation/RoomLayout.h"
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

Room& World::GetRoomAt(Coords coords)
{
    if (coords.GetX() >= MaximumSpan || coords.GetY() >= MaximumSpan)
    {
        std::ostringstream errorMessage;
        errorMessage << "World grid position out of bounds: "
                     << coords;
        throw std::out_of_range(errorMessage.str());
    }

    if (m_Rooms[coords.GetX()][coords.GetY()] == nullptr)
    {
        std::ostringstream errorMessage;
        errorMessage << "Room "
                     << coords
                     << " of world "
                     << m_WorldNumber
                     << " is uninitialized";
        throw std::invalid_argument(errorMessage.str());
    }

    return *m_Rooms[coords.GetX()][coords.GetY()];
}

const Room& World::GetRoomAt(Coords coords) const
{
    if (coords.GetX() >= MaximumSpan || coords.GetY() >= MaximumSpan)
    {
        std::ostringstream errorMessage;
        errorMessage << "World grid position out of bounds: "
                     << coords;
        throw std::out_of_range(errorMessage.str());
    }

    if (m_Rooms[coords.GetX()][coords.GetY()] == nullptr)
    {
        std::ostringstream errorMessage;
        errorMessage << "Room "
                     << coords
                     << " of world "
                     << m_WorldNumber
                     << " is uninitialized";
        throw std::invalid_argument(errorMessage.str());
    }

    return *m_Rooms[coords.GetX()][coords.GetY()];
}

bool World::IsPositionAtWorldGridEdge(Coords coords, Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return coords.GetY() == 0;
    case Direction::Value::Right:
        return coords.GetX() == MaximumSpan - 1;
    case Direction::Value::Down:
        return coords.GetY() == MaximumSpan - 1;
    case Direction::Value::Left:
        return coords.GetX() == 0;
    default:
        return false;
    }
}

Room& World::GetStartingRoom()
{
    return GetRoomAt({ CenterPos, CenterPos });
}

const Room& World::GetStartingRoom() const
{
    return GetRoomAt({ CenterPos, CenterPos });
}

Room& World::CreateRoomAt(Coords coords)
{
    auto layout = m_RoomGenerator.CreateLayout(coords);
    m_Rooms[coords.GetX()][coords.GetY()] = std::make_unique<Room>(
        m_WorldManager,
        *this,
        *layout,
        PopNextRoomNumber(),
        coords);

    return *m_Rooms[coords.GetX()][coords.GetY()];
}

bool World::RoomExistsAt(Coords coords) const
{
    return m_Rooms[coords.GetX()][coords.GetY()] != nullptr;
}

int World::PopNextRoomNumber()
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
        PopNextRoomNumber(),
        coords);
}

} /* namespace Worlds */
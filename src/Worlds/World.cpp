#include "World.h"
#include "Misc/Coords.h"
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
      m_WorldNumber(worldNumber),
      m_NextRoomNumber(1)
{
    m_Rooms.resize(MaxSpan);
    for (auto& vec : m_Rooms)
        vec.resize(MaxSpan);
    CreateRoomAt({ CenterPos, CenterPos });
}

int World::GetWorldNumber() const
{
    return m_WorldNumber;
}

Room& World::GetRoomAt(Coords coords)
{
    if (coords.GetX() >= MaxSpan || coords.GetY() >= MaxSpan)
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
    if (coords.GetX() >= MaxSpan || coords.GetY() >= MaxSpan)
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
    m_Rooms[coords.GetX()][coords.GetY()] = std::make_unique<Room>(
        m_WorldManager,
        *this,
        PopNextRoomNumber(),
        coords);

    m_Rooms[coords.GetX()][coords.GetY()]->Generate(Layout::RandLayout);
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

} /* namespace Worlds */
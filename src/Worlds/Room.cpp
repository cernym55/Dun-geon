#include "Room.h"
#include "Entities/Player.h"
#include "Entities/StaticEntities.h"
#include "Field.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Misc/RNG.h"
#include "World.h"
#include "WorldManager.h"
#include <array>
#include <sstream>

#define MAX_WIDTH 40
#define MAX_HEIGHT 20
#define MAX_WIDTH_DIFF 15
#define MAX_HEIGHT_DIFF 5

namespace Worlds
{

Room::Room(WorldManager& worldManager, World& world, int roomNumber, Coords coords)
    : m_WorldManager(worldManager),
      m_World(world),
      m_RoomNumber(roomNumber),
      m_Coords(coords),
      m_Entrances({ nullptr })
{
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

int Room::GetRoomNumber() const
{
    return m_RoomNumber;
}

bool Room::IsAtWorldGridEdge(Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return m_Coords.GetY() == 0;
    case Direction::Value::Right:
        return m_Coords.GetX() == m_World.MaxSpan - 1;
    case Direction::Value::Down:
        return m_Coords.GetY() == m_World.MaxSpan - 1;
    case Direction::Value::Left:
        return m_Coords.GetX() == 0;
    default:
        return false;
    }
}

const Field* Room::TryGetEntrance(Direction dir) const
{
    return m_Entrances[(int)dir()];
}

bool Room::HasNeighbor(Direction dir) const
{
    if (IsAtWorldGridEdge(dir)) return false;

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

void Room::Generate(Layout layout, std::array<bool, 4> forceEntrances)
{
    // Fenerate random dimensions (within constraints)
    m_Width = RNG::RandomInt(MAX_WIDTH_DIFF) + MAX_WIDTH - MAX_WIDTH_DIFF;
    m_Width += m_Width % 2;
    m_Height = RNG::RandomInt(MAX_HEIGHT_DIFF) + MAX_HEIGHT - MAX_HEIGHT_DIFF;
    m_Height += m_Height % 2;

    // Fill map with wall fields
    m_Fields.resize(m_Width);
    for (size_t i = 0; i < m_Width; i++)
    {
        for (size_t j = 0; j < m_Height; j++)
        {
            m_Fields[i].emplace_back(Coords(i, j));
            m_Fields[i].back().PlaceEntity(Entities::Wall);
        }
    }

    // By default, an entrance *may* be generated
    std::array<bool, 4> allowEntrances = { true, true, true, true };

    for (auto& dir : Direction::All())
    {
        // If there can be no neighbor, forbid entrances
        if (IsAtWorldGridEdge(dir))
        {
            allowEntrances[dir.ToInt()] = false;
        }
        else if (HasNeighbor(dir))
        {
            // If there is a neighbor with an entrance, force one here
            if (GetNeighbor(dir).TryGetEntrance(dir.Opposite()) != nullptr)
            {
                forceEntrances[dir.ToInt()] = true;
            }
            // If there is a neighbor without an entrance, forbid one here
            else
            {
                allowEntrances[dir.ToInt()] = false;
            }
        }
    }

    // Generate layout
    int layoutNum;
    if (layout == Layout::RandLayout)
    {
        //layoutNum = RNG(0, NUM_LAYOUTS); TODO: uncomment after more layouts are added (don't ask why)
        layoutNum = 0;
    }
    else
    {
        layoutNum = static_cast<int>(layout);
    }
    switch (layoutNum)
    {
    case static_cast<int>(Layout::Box):
        for (size_t i = 1; i < m_Width - 1; i++)
        {
            for (size_t j = 1; j < m_Height - 1; j++)
            {
                m_Fields[i][j].VacateForeground();
            }
        }
        // Generate entrances (check if room is not on edge of map and if neighbor rooms can be connected)
        if (allowEntrances[Direction::Up().ToInt()] &&
            (forceEntrances[Direction::Up().ToInt()] || RNG::Chance(0.5)))
        {
            auto& entranceUp = m_Entrances[Direction::Up().ToInt()];
            entranceUp = &GetFieldAt({ RNG::RandomInt(3, m_Width - 3), 0 });
            entranceUp->VacateForeground();
            GetFieldAt({ entranceUp->GetCoords().GetX() - 1, 0 }).VacateForeground();
            GetFieldAt({ entranceUp->GetCoords().GetX() + 1, 0 }).VacateForeground();
        }
        if (allowEntrances[Direction::Right().ToInt()] &&
            (forceEntrances[Direction::Right().ToInt()] || RNG::Chance(0.5)))
        {
            auto& entranceRight = m_Entrances[Direction::Right().ToInt()];
            entranceRight = &GetFieldAt({ m_Width - 1, RNG::RandomInt(3, m_Height - 3) });
            entranceRight->VacateForeground();
            GetFieldAt({ m_Width - 1, entranceRight->GetCoords().GetY() - 1 }).VacateForeground();
            GetFieldAt({ m_Width - 1, entranceRight->GetCoords().GetY() + 1 }).VacateForeground();
        }
        if (allowEntrances[Direction::Down().ToInt()] &&
            (forceEntrances[Direction::Down().ToInt()] || RNG::Chance(0.5)))
        {
            auto& entranceDown = m_Entrances[Direction::Down().ToInt()];
            entranceDown = &GetFieldAt({ RNG::RandomInt(3, m_Width - 3), m_Height - 1 });
            entranceDown->VacateForeground();
            GetFieldAt({ entranceDown->GetCoords().GetX() - 1, m_Height - 1 }).VacateForeground();
            GetFieldAt({ entranceDown->GetCoords().GetX() + 1, m_Height - 1 }).VacateForeground();
        }
        if (allowEntrances[Direction::Left().ToInt()] &&
            (forceEntrances[Direction::Left().ToInt()] || RNG::Chance(0.5)))
        {
            auto& entranceLeft = m_Entrances[Direction::Left().ToInt()];
            entranceLeft = &GetFieldAt({ 0, RNG::RandomInt(3, m_Height - 3) });
            entranceLeft->VacateForeground();
            GetFieldAt({ 0, entranceLeft->GetCoords().GetY() - 1 }).VacateForeground();
            GetFieldAt({ 0, entranceLeft->GetCoords().GetY() + 1 }).VacateForeground();
        }
        break;
    }
}

} /* namespace Worlds */
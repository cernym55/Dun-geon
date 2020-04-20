#include "Room.h"
#include "Entities/Player.h"
#include "Entities/StaticEntities.h"
#include "Field.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Misc/RNG.h"
#include "World.h"
#include "WorldManager.h"
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
      m_Coords(coords)
{
    entranceUp = nullptr;
    entranceLeft = nullptr;
    entranceRight = nullptr;
    entranceDown = nullptr;
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

bool Room::HasEntrance(Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return entranceUp != nullptr;
    case Direction::Value::Right:
        return entranceRight != nullptr;
    case Direction::Value::Down:
        return entranceDown != nullptr;
    case Direction::Value::Left:
        return entranceLeft != nullptr;
    default:
        return false;
    }
}

const Field& Room::GetEntrance(Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return *entranceUp;
        break;
    case Direction::Value::Right:
        return *entranceRight;
        break;
    case Direction::Value::Down:
        return *entranceDown;
        break;
    case Direction::Value::Left:
        return *entranceLeft;
        break;
    default: {
        std::ostringstream errorMessage;
        errorMessage << "Requested nonexistent entrance field in direction "
                     << dir;
        throw std::invalid_argument(errorMessage.str());
    }
    }
}

bool Room::HasNeighbor(Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return roomUp != nullptr;
    case Direction::Value::Right:
        return roomRight != nullptr;
    case Direction::Value::Down:
        return roomDown != nullptr;
    case Direction::Value::Left:
        return roomLeft != nullptr;
    default:
        return false;
    }
}

const Room& Room::GetNeighbor(Direction dir) const
{
    switch (dir())
    {
    case Direction::Value::Up:
        return *roomUp;
    case Direction::Value::Right:
        return *roomRight;
    case Direction::Value::Down:
        return *roomDown;
    case Direction::Value::Left:
        return *roomLeft;
    default: {
        std::ostringstream errorMessage;
        errorMessage << "Requested nonexistent neighboring room in direction "
                     << dir;
        throw std::invalid_argument(errorMessage.str());
    }
    }
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
        return fields[coords.GetX()][coords.GetY()];
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
        return fields[coords.GetX()][coords.GetY()];
    }
}

void Room::generate(Layout layout, bool forceUp, bool forceRight, bool forceDown, bool forceLeft)
{
    // generate random dimensions (within constraints)
    m_Width = RNG::RandomInt(MAX_WIDTH_DIFF) + MAX_WIDTH - MAX_WIDTH_DIFF;
    m_Width += m_Width % 2;
    m_Height = RNG::RandomInt(MAX_HEIGHT_DIFF) + MAX_HEIGHT - MAX_HEIGHT_DIFF;
    m_Height += m_Height % 2;

    // fill map with wall fields
    fields.resize(m_Width);
    for (size_t i = 0; i < m_Width; i++)
    {
        for (size_t j = 0; j < m_Height; j++)
        {
            fields[i].emplace_back(Coords(i, j));
            fields[i].back().PlaceEntity(Entities::Wall);
        }
    }

    // check if forced entrances are needed/allowed
    bool allowUp = true, allowRight = true, allowDown = true, allowLeft = true;
    if (!HasNeighbor(Direction::Up()))
    {
        allowUp = false;
        forceUp = false;
    }
    else
    {
        GetNeighbor(Direction::Up()).HasEntrance(Direction::Down()) ? allowUp = false : forceUp = true;
    }

    if (!HasNeighbor(Direction::Right()))
    {
        allowRight = false;
        forceRight = false;
    }
    else
    {
        GetNeighbor(Direction::Right()).HasEntrance(Direction::Left()) ? allowRight = false : forceRight = true;
    }

    if (!HasNeighbor(Direction::Down()))
    {
        allowDown = false;
        forceDown = false;
    }
    else
    {
        GetNeighbor(Direction::Down()).HasEntrance(Direction::Up()) ? allowDown = false : forceDown = true;
    }

    if (!HasNeighbor(Direction::Left()))
    {
        allowLeft = false;
        forceLeft = false;
    }
    else
    {
        GetNeighbor(Direction::Left()).HasEntrance(Direction::Right()) ? allowLeft = false : forceLeft = true;
    }

    // generate layout
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
                fields[i][j].VacateForeground();
            }
        }
        // generate entrances (check if room is not on edge of map and if neighbor rooms can be connected)
        if (allowUp && (forceUp || RNG::Chance(0.5)))
        {
            entranceUp = &GetFieldAt({ RNG::RandomInt(3, m_Width - 3), 0 });
            entranceUp->VacateForeground();
            GetFieldAt({ entranceUp->GetCoords().GetX() - 1, 0 }).VacateForeground();
            GetFieldAt({ entranceUp->GetCoords().GetX() + 1, 0 }).VacateForeground();
        }
        if (allowDown && (forceDown || RNG::Chance(0.5)))
        {
            entranceDown = &GetFieldAt({ RNG::RandomInt(3, m_Width - 3), m_Height - 1 });
            entranceDown->VacateForeground();
            GetFieldAt({ entranceDown->GetCoords().GetX() - 1, m_Height - 1 }).VacateForeground();
            GetFieldAt({ entranceDown->GetCoords().GetX() + 1, m_Height - 1 }).VacateForeground();
        }
        if (allowLeft && (forceLeft || RNG::Chance(0.5)))
        {
            entranceLeft = &GetFieldAt({ 0, RNG::RandomInt(3, m_Height - 3) });
            entranceLeft->VacateForeground();
            GetFieldAt({ 0, entranceLeft->GetCoords().GetY() - 1 }).VacateForeground();
            GetFieldAt({ 0, entranceLeft->GetCoords().GetY() + 1 }).VacateForeground();
        }
        if (allowRight && (forceRight || RNG::Chance(0.5)))
        {
            entranceRight = &GetFieldAt({ m_Width - 1, RNG::RandomInt(3, m_Height - 3) });
            entranceRight->VacateForeground();
            GetFieldAt({ m_Width - 1, entranceRight->GetCoords().GetY() - 1 }).VacateForeground();
            GetFieldAt({ m_Width - 1, entranceRight->GetCoords().GetY() + 1 }).VacateForeground();
        }
        break;
    }
}

} /* namespace Worlds */
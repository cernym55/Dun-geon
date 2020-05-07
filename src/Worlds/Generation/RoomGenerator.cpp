#include "RoomGenerator.h"
#include "../Room.h"
#include "../World.h"
#include "BoxRoomLayout.h"
#include "HallwayRoomLayout.h"
#include "Misc/Coords.h"
#include "Misc/RNG.h"
#include "RoomGenerationParameters.h"
#include "RoomLayout.h"
#include <memory>

namespace Worlds
{

namespace Generation
{

RoomGenerator::RoomGenerator(World& world)
    : m_World(world),
      m_GeneratedRoomCount(0),
      m_UndiscoveredRoomCount(0)
{
    InitializeParameters();
}

std::unique_ptr<RoomLayout> RoomGenerator::CreateLayout(Coords coords)
{
    int layoutNumber = RNG::RandomInt(RoomLayout::NumberOfTypes);
    return CreateLayout(static_cast<RoomLayout::Type>(layoutNumber), coords);
}

std::unique_ptr<RoomLayout> RoomGenerator::CreateLayout(RoomLayout::Type layoutType, Coords coords)
{
    std::unique_ptr<RoomLayout> layout;
    m_Parameters.EntranceInfo = GetEntranceInfoByCoords(coords);

    switch (layoutType)
    {
    case RoomLayout::Type::Box:
        layout = std::make_unique<BoxRoomLayout>(m_Parameters);
        break;
    case RoomLayout::Type::Hallway:
        layout = std::make_unique<HallwayRoomLayout>(m_Parameters);
        break;
    }

    if (m_GeneratedRoomCount > 0) m_UndiscoveredRoomCount--;
    m_GeneratedRoomCount++;
    m_UndiscoveredRoomCount += layout->GetEntrances().size() - 1;
    UpdateParameters();

    return layout;
}

void RoomGenerator::InitializeParameters()
{
    m_Parameters.ForceContinue = true;
    m_Parameters.OptionalEntranceChance = 0.5;
    m_Parameters.DarknessChance = 0;
}

void RoomGenerator::UpdateParameters()
{
    m_Parameters.ForceContinue = false;
    m_Parameters.OptionalEntranceChance = 0.5;
    int roomCountFloor = RoomCountFloor();
    int roomCountCap = RoomCountCap();
    if (m_UndiscoveredRoomCount - 1 <= m_GeneratedRoomCount / 10 && m_GeneratedRoomCount < roomCountFloor)
    {
        // When running out of rooms
        m_Parameters.ForceContinue = true;
    }
    else if (m_GeneratedRoomCount + m_UndiscoveredRoomCount >= roomCountCap)
    {
        // When the room cap is hit
        m_Parameters.OptionalEntranceChance = 0;
    }
    else if (m_GeneratedRoomCount >= roomCountFloor && m_GeneratedRoomCount <= roomCountCap)
    {
        // When we're between the room floor and cap
        m_Parameters.OptionalEntranceChance = (roomCountCap - m_GeneratedRoomCount) / 100.0;
    }

    m_Parameters.DarknessChance = 0.1;
    for (int i = 0; i < m_World.GetWorldNumber(); i++)
        m_Parameters.DarknessChance += 0.015;
}

int RoomGenerator::RoomCountFloor() const
{
    int base = 36;
    for (int i = 0; i < m_World.GetWorldNumber(); i++)
    {
        base *= 1.2;
    }
    return base;
}

int RoomGenerator::RoomCountCap() const
{
    int base = 50;
    for (int i = 0; i < m_World.GetWorldNumber(); i++)
    {
        base *= 1.2;
    }
    return base;
}

std::map<Direction, bool> RoomGenerator::GetEntranceInfoByCoords(Coords coords) const
{
    std::map<Direction, bool> entranceInfo;

    for (auto& dir : Direction::All())
    {
        // If there can be no neighbor, forbid entrances
        if (m_World.IsPositionAtWorldGridEdge(coords, dir))
        {
            entranceInfo[dir] = false;
        }
        else if (m_World.RoomExistsAt(coords.GetAdjacent(dir)))
        {
            // If there is a neighbor with an entrance, force one here
            if (m_World.GetRoomAt(coords.GetAdjacent(dir)).TryGetEntrance(dir.Opposite()) != nullptr)
            {
                entranceInfo[dir] = true;
            }
            // If there is a neighbor without an entrance, forbid one here
            else
            {
                entranceInfo[dir] = false;
            }
        }
    }

    return entranceInfo;
}

} /* namespace Generation */

} /* namespace Worlds */
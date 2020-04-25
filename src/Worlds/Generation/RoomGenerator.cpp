#include "RoomGenerator.h"
#include "../Room.h"
#include "../World.h"
#include "BoxRoomLayout.h"
#include "Misc/Coords.h"
#include "Misc/RNG.h"
#include "RoomLayout.h"
#include <memory>

namespace Worlds
{

namespace Generation
{

RoomGenerator::RoomGenerator(World& world)
    : m_World(world)
{
}

std::unique_ptr<RoomLayout> RoomGenerator::CreateLayout(Coords coords)
{
    int layoutNumber = RNG::RandomInt(RoomLayout::NumberOfTypes);
    return CreateLayout(static_cast<RoomLayout::Type>(layoutNumber), coords);
}

std::unique_ptr<RoomLayout> RoomGenerator::CreateLayout(RoomLayout::Type layoutType, Coords coords)
{
    std::unique_ptr<RoomLayout> layout;
    auto entranceInfo = GetEntranceInfoByCoords(coords);

    switch (layoutType)
    {
    case RoomLayout::Type::Box:
        layout = std::make_unique<BoxRoomLayout>(entranceInfo);
    }

    return layout;
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
#include "HallwayRoomLayout.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Misc/RNG.h"
#include "Misc/Utils.h"
#include "RoomGenerationParameters.h"
#include "UI/CameraStyle.h"
#include <map>

namespace Worlds
{

namespace Generation
{

HallwayRoomLayout::HallwayRoomLayout(const RoomGenerationParameters& parameters)
    : RoomLayout(parameters)
{
    Generate();
}

void HallwayRoomLayout::Generate()
{
    GenerateAttributes();

    m_Map.resize(m_Width);
    for (auto& col : m_Map)
    {
        col.resize(m_Height);
    }

    // Generate entrance positions
    std::map<Direction, Coords> allEntrances;

    auto actualEntranceDirections = GenerateEntranceDirections();
    for (const auto& dir : actualEntranceDirections)
    {
        m_Entrances[dir] = {};
    }

    for (const auto& dir : Direction::All())
    {
        allEntrances[dir] = GenerateEntranceCoords(dir);
    }

    // Wall positioning algorithm
    // Hallways may branch, but they must be perfectly straight either horizontally or vertically.
    bool vertical = RNG::Chance(0.5);

    // The two entrance positions on the straight axis must thus share a coordinate.
    if (vertical)
    {
        allEntrances[Direction::Down()].SetX(allEntrances[Direction::Up()].GetX());
    }
    else
    {
        allEntrances[Direction::Right()].SetY(allEntrances[Direction::Left()].GetY());
    }

    // Load the pre-generated entrance positions for the entrances we'll be keeping.
    for (auto& entrance : m_Entrances)
    {
        entrance.second = allEntrances[entrance.first];
    }

    // The central portion of the hallway between the opposing entrance approach sections
    // on the straight axis is always present, we'll call it the "spine."
    size_t spineCoord;        // This coord can be X or Y depending on the axis orientation
    std::vector<Coords> path; // Coords outlining the pathway for the player
    Coords spineStart, spineEnd;

    if (vertical)
    {
        spineCoord = allEntrances[Direction::Up()].GetX();
        path = Coords(spineCoord, allEntrances[Direction::Left()].GetY())
                   .StraightPathTo({ spineCoord, allEntrances[Direction::Right()].GetY() });
    }
    else
    {
        spineCoord = allEntrances[Direction::Left()].GetY();
        path = Coords(allEntrances[Direction::Up()].GetX(), spineCoord)
                   .StraightPathTo({ allEntrances[Direction::Down()].GetX(), spineCoord });
    }

    spineStart = path.front();
    spineEnd = path.back();

    // Add entrance approaches to the path
    if (m_Entrances.count(Direction::Up()) > 0)
    {
        auto entrance = m_Entrances[Direction::Up()];
        if (vertical)
        {
            for (const auto& coords : entrance.StraightPathTo({ spineCoord, Min(spineStart.GetY(), spineEnd.GetY()) - 1 }))
            {
                path.push_back(coords);
            }
        }
        else
        {
            for (const auto& coords : entrance.StraightPathTo({ entrance.GetX(), spineCoord - 1 }))
            {
                path.push_back(coords);
            }
        }
    }

    if (m_Entrances.count(Direction::Right()) > 0)
    {
        auto entrance = m_Entrances[Direction::Right()];
        if (vertical)
        {
            for (const auto& coords : entrance.StraightPathTo({ spineCoord + 1, entrance.GetY() }))
            {
                path.push_back(coords);
            }
        }
        else
        {
            for (const auto& coords : entrance.StraightPathTo({ Max(spineStart.GetX(), spineEnd.GetX()) + 1, spineCoord }))
            {
                path.push_back(coords);
            }
        }
    }

    if (m_Entrances.count(Direction::Down()) > 0)
    {
        auto entrance = m_Entrances[Direction::Down()];
        if (vertical)
        {
            for (const auto& coords : entrance.StraightPathTo({ spineCoord, Max(spineStart.GetY(), spineEnd.GetY()) + 1 }))
            {
                path.push_back(coords);
            }
        }
        else
        {
            for (const auto& coords : entrance.StraightPathTo({ entrance.GetX(), spineCoord + 1 }))
            {
                path.push_back(coords);
            }
        }
    }

    if (m_Entrances.count(Direction::Left()) > 0)
    {
        auto entrance = m_Entrances[Direction::Left()];
        if (vertical)
        {
            for (const auto& coords : entrance.StraightPathTo({ spineCoord - 1, entrance.GetY() }))
            {
                path.push_back(coords);
            }
        }
        else
        {
            for (const auto& coords : entrance.StraightPathTo({ Min(spineStart.GetX(), spineEnd.GetX()) - 1, spineCoord }))
            {
                path.push_back(coords);
            }
        }
    }

    // The box drawing approach is rather slow but produces good results.
    // We can remedy this to an extent by only drawing every other box.
    // This can produce a cool "rounded corner" effect sometimes but won't break anything.
    bool stagger = false;

    // Surround the path with walls
    for (const auto& coords : path)
    {
        if (!stagger)
            DrawMapBox(coords, HallwayWidth / 2 + 1);
        stagger = !stagger;
    }

    stagger = false;
    // Clear everything inside the walls
    for (const auto& coords : path)
    {
        if (!stagger)
            DrawMapBox(coords, HallwayWidth / 2, FieldType::Accessible);
        stagger = !stagger;
    }
}

void HallwayRoomLayout::GenerateAttributes()
{
    RoomLayout::GenerateAttributes();

    // Randomize dimensions, make sure they're an even number
    m_Width = RNG::RandomInt(MinimumWidth, MaximumWidth);
    m_Width += m_Width % 2;
    m_Height = RNG::RandomInt(MinimumHeight, MaximumHeight);
    m_Height += m_Height % 2;

    m_CameraStyle = UI::CameraStyle::PlayerCentered;
    // Generate lighting
    if (RNG::Chance(m_Parameters.DarknessChance))
    {
        m_VisionRadius = DarknessVisionRadius;
    }
}

} /* namespace Generation */

} /* namespace Worlds */
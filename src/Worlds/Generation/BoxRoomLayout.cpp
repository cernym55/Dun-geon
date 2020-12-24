#include "BoxRoomLayout.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Misc/RNG.h"
#include "Misc/Utils.h"
#include "RoomGenerationParameters.h"
#include "UI/CameraStyle.h"
#include <algorithm>
#include <map>

namespace Worlds::Generation
{

BoxRoomLayout::BoxRoomLayout(const RoomGenerationParameters& parameters)
    : RoomLayout(parameters), m_Subtype(Subtype::Default)
{
    BoxRoomLayout::Generate();
}

void BoxRoomLayout::Generate()
{
    GenerateAttributes();

    m_Map.resize(m_Width);
    for (auto& col : m_Map)
    {
        col.resize(m_Height);
    }

    // Generate entrance positions
    std::vector<Coords> entranceCoords;

    auto entranceDirections = GenerateEntranceDirections();
    for (const auto& dir : entranceDirections)
    {
        m_Entrances[dir] = GenerateEntranceCoords(dir);
        entranceCoords.push_back(m_Entrances[dir]);
    }

    // Build walls
    for (Coords::Scalar col = 0; col < m_Width; col++)
    {
        for (Coords::Scalar row = 0; row < m_Height; row++)
        {
            m_Map[col][row] = FieldType::Accessible;
            if ((col > 0 && row > 0 && col < m_Width - 1 && row < m_Height - 1))
            {
                continue;
            }

            if (row == 0 && m_Entrances.count(Direction::Up) > 0 && Abs(col - m_Entrances[Direction::Up].X) <= EntranceRadius)
            {
                continue;
            }
            if (col == m_Width - 1 && m_Entrances.count(Direction::Right) > 0 && Abs(row - m_Entrances[Direction::Right].Y) <= EntranceRadius)
            {
                continue;
            }
            if (row == m_Height - 1 && m_Entrances.count(Direction::Down) > 0 && Abs(col - m_Entrances[Direction::Down].X) <= EntranceRadius)
            {
                continue;
            }
            if (col == 0 && m_Entrances.count(Direction::Left) > 0 && Abs(row - m_Entrances[Direction::Left].Y) <= EntranceRadius)
            {
                continue;
            }

            m_Map[col][row] = FieldType::Wall;
        }
    }

    // Generate subtype contents
    switch (m_Subtype)
    {
    case Subtype::ColumnsOnly:
        GenerateColumns();
        break;
    default:
        break;
    }
}

void BoxRoomLayout::GenerateAttributes()
{
    RoomLayout::GenerateAttributes();

    // Randomize dimensions, make sure they're an even number
    m_Width = RNG::RandomInt(MinimumWidth, MaximumWidth);
    m_Width += m_Width % 2;
    m_Height = RNG::RandomInt(MinimumHeight, MaximumHeight);
    m_Height += m_Height % 2;

    m_CameraStyle = UI::CameraStyle::Fixed;
    // Generate lighting
    if (RNG::Chance(m_Parameters.DarknessChance))
    {
        m_VisionRadius = DarknessVisionRadius;
    }

    // Randomize room subtype
    double subtypeRoll = RNG::RandomDouble();
    if (subtypeRoll > 0.75)
    {
        m_Subtype = Subtype::ColumnsOnly;
    }

    m_NPCSpawnChance = 0.55;
}

void BoxRoomLayout::GenerateColumns()
{
    double pattern = RNG::RandomDouble();

    Coords::Scalar hOffset = RNG::RandomInt(2, 5);
    Coords::Scalar vOffset = 2;

    // Corner columns
    if (pattern > 0.4)
    {
        m_Map[hOffset][vOffset]                              = FieldType::Column;
        m_Map[m_Width - hOffset - 1][vOffset]                = FieldType::Column;
        m_Map[hOffset][m_Height - vOffset - 1]               = FieldType::Column;
        m_Map[m_Width - hOffset - 1][m_Height - vOffset - 1] = FieldType::Column;
    }

    // Center columns
    if (pattern < 0.6)
    {
        Coords::Scalar hCenter = m_Width / 2;
        Coords::Scalar vCenter = m_Height / 2;

        if (RNG::Chance(0.5)) vOffset--;

        m_Map[hCenter - hOffset - 1][vCenter - vOffset - 1] = FieldType::Column;
        m_Map[hCenter + hOffset][vCenter - vOffset - 1]     = FieldType::Column;
        m_Map[hCenter - hOffset - 1][vCenter + vOffset]     = FieldType::Column;
        m_Map[hCenter + hOffset][vCenter + vOffset]         = FieldType::Column;
    }
}

} /* namespace Worlds::Generation */

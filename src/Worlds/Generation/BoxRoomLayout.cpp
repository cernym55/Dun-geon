#include "BoxRoomLayout.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Misc/RNG.h"
#include "Misc/Utils.h"
#include <algorithm>
#include <map>

namespace Worlds
{

namespace Generation
{

BoxRoomLayout::BoxRoomLayout(const std::map<Direction, bool>& entranceInfo)
    : RoomLayout(entranceInfo)
{
    Generate();
}

void BoxRoomLayout::Generate()
{
    // Randomize dimensions, make sure they're an even number
    m_Width = RNG::RandomInt(MinimumWidth, MaximumWidth);
    m_Width += m_Width % 2;
    m_Height = RNG::RandomInt(MinimumHeight, MaximumHeight);
    m_Height += m_Height % 2;
    m_Map.resize(m_Width);
    for (auto& col : m_Map)
    {
        col.resize(m_Height);
    }

    // Generate entrance positions
    std::vector<Coords> entranceCoords;

    for (const auto& dir : Direction::All())
    {
        if (m_EntranceInfo.count(dir) > 0 && m_EntranceInfo.at(dir) == false)
        {
            continue;
        }
        else if ((m_EntranceInfo.count(dir) > 0 && m_EntranceInfo.at(dir) == true) || RNG::Chance(0.5))
        {
            m_Entrances[dir] = GenerateEntranceCoords(dir);
            entranceCoords.push_back(m_Entrances[dir]);
        }
    }

    // Build walls
    for (size_t col = 0; col < m_Width; col++)
    {
        for (size_t row = 0; row < m_Height; row++)
        {
            if ((col > 0 && row > 0 && col < m_Width - 1 && row < m_Height - 1))
            {
                continue;
            }

            if (row == 0 && m_Entrances.count(Direction::Up()) > 0 && Abs(col - m_Entrances[Direction::Up()].GetX()) <= EntranceRadius)
            {
                continue;
            }
            if (col == m_Width - 1 && m_Entrances.count(Direction::Right()) > 0 && Abs(row - m_Entrances[Direction::Right()].GetY()) <= EntranceRadius)
            {
                continue;
            }
            if (row == m_Height - 1 && m_Entrances.count(Direction::Down()) > 0 && Abs(col - m_Entrances[Direction::Down()].GetX()) <= EntranceRadius)
            {
                continue;
            }
            if (col == 0 && m_Entrances.count(Direction::Left()) > 0 && Abs(row - m_Entrances[Direction::Left()].GetY()) <= EntranceRadius)
            {
                continue;
            }

            m_Map[col][row] = true;
        }
    }
}

} /* namespace Generation */

} /* namespace Worlds */
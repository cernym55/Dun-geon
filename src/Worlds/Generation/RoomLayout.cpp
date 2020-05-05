#include "RoomLayout.h"
#include "Entities/StaticEntities.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Misc/Exceptions.h"
#include "Misc/RNG.h"
#include <map>
#include <vector>

namespace Worlds
{

namespace Generation
{

RoomLayout::RoomLayout(const std::map<Direction, bool>& entranceInfo)
    : m_EntranceInfo(entranceInfo)
{
}

void RoomLayout::GenerateAttributes()
{
}

void RoomLayout::WriteToFields(std::vector<std::vector<Field>>& fields) const
{
    fields.clear();
    fields.resize(m_Width);
    for (size_t i = 0; i < m_Width; i++)
    {
        for (size_t j = 0; j < m_Height; j++)
        {
            fields[i].emplace_back(Coords(i, j));
            if (m_Map[i][j] == true)
            {
                fields[i][j].PlaceEntity(Entities::Wall);
            }
        }
    }
}

const std::map<Direction, Coords>& RoomLayout::GetEntrances() const
{
    return m_Entrances;
}

RoomLayout::CameraStyle RoomLayout::GetCameraStyle() const
{
    return m_CameraStyle;
}

Coords RoomLayout::GenerateEntranceCoords(Direction dir)
{
    constexpr static const int MinimumCornerDistance = 3;
    switch (dir())
    {
    case Direction::Value::Up:
        return Coords(RNG::RandomInt(MinimumCornerDistance, m_Width - MinimumCornerDistance), 0);
    case Direction::Value::Right:
        return Coords(m_Width - 1, RNG::RandomInt(MinimumCornerDistance, m_Height - MinimumCornerDistance));
    case Direction::Value::Down:
        return Coords(RNG::RandomInt(MinimumCornerDistance, m_Width - MinimumCornerDistance), m_Height - 1);
    case Direction::Value::Left:
        return Coords(0, RNG::RandomInt(MinimumCornerDistance, m_Height - MinimumCornerDistance));
    default:
        throw InvalidPositionException("Attempted to generate entrance coords for direction None");
    }
}

void RoomLayout::DrawMapLine(Coords from, Coords to, bool value)
{
    for (const auto& pos : from.StraightPathTo(to))
    {
        m_Map[pos.GetX()][pos.GetY()] = value;
    }
}

void RoomLayout::DrawMapBox(Coords center, size_t radius, bool value)
{
    for (size_t i = 0; i <= radius; i++)
    {
        for (size_t j = 0; j <= radius; j++)
        {
            size_t left = center.GetX() - i;
            size_t right = center.GetX() + i;
            size_t up = center.GetY() - j;
            size_t down = center.GetY() + j;
            if (left < m_Width)
            {
                if (up < m_Height) m_Map[left][up] = value;
                if (down < m_Height) m_Map[left][down] = value;
            }
            if (right < m_Width)
            {
                if (up < m_Height) m_Map[right][up] = value;
                if (down < m_Height) m_Map[right][down] = value;
            }
        }
    }
}

} /* namespace Generation */

} /* namespace Worlds */
#include "RoomLayout.h"
#include "Entities/StaticEntities.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
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

} /* namespace Generation */

} /* namespace Worlds */
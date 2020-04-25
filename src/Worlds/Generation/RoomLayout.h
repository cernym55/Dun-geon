#pragma once

#include "Field.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include <map>
#include <unordered_map>
#include <vector>

namespace Worlds
{

namespace Generation
{

/**
 * @brief Contains a description of the interior of a room which can be randomly generated
 */
class RoomLayout
{
public:
    /**
     * @brief Types of room layouts
     */
    enum class Type
    {
        /**
         * @brief Rectangular layout with straight walls and up to 4 entrances
         */
        Box = 0
    };

    /**
     * @brief How many layout types there are
     */
    constexpr static const int NumberOfTypes = 1;

    /**
     * @brief Radius around an entrance field which should be kept empty
     */
    constexpr static const int EntranceRadius = 1;

    RoomLayout(const RoomLayout&) = delete;

    RoomLayout(RoomLayout&&) = delete;

    RoomLayout& operator=(const RoomLayout&) = delete;

    RoomLayout& operator=(RoomLayout&&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~RoomLayout() = default;

    /**
     * @brief Write the layout to a 2D vector of fields
     * 
     * @param fields fields
     */
    void WriteToFields(std::vector<std::vector<Field>>& fields) const;

    /**
     * @brief Get a map of entrance coords per direction
     * 
     * @return const std::map<Direction, Coords>& map of entrance coords
     */
    const std::map<Direction, Coords>& GetEntrances() const;

protected:
    size_t m_Width;
    size_t m_Height;
    std::vector<std::vector<bool>> m_Map;
    const std::map<Direction, bool>& m_EntranceInfo;
    std::map<Direction, Coords> m_Entrances;

    /**
     * @brief Constructor
     * 
     * @param entranceInfo entrance info
     */
    RoomLayout(const std::map<Direction, bool>& entranceInfo);

    /**
     * @brief Generate the layout
     */
    virtual void Generate() = 0;
};

} /* namespace Generation */

} /* namespace Worlds */
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
        Box = 0,

        /**
         * @brief Hallway leading from one entrance to another, can branch
         */
        Hallway = 1
    };

    /**
     * @brief Camera style used when viewing this room
     */
    enum class CameraStyle
    {
        /**
         * @brief Fixed view of the entire room
         */
        Fixed,

        /**
         * @brief Player is kept in the center of the view
         */
        PlayerCentered
    };

    /**
     * @brief How many layout types there are
     */
    constexpr static const int NumberOfTypes = 2;

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

    /**
     * @brief Get the camera style
     * 
     * @return CameraStyle camera style
     */
    CameraStyle GetCameraStyle() const;

protected:
    size_t m_Width;
    size_t m_Height;
    std::vector<std::vector<bool>> m_Map;
    const std::map<Direction, bool>& m_EntranceInfo;
    std::map<Direction, Coords> m_Entrances;
    CameraStyle m_CameraStyle;

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

    /**
     * @brief Generate room attributes not related to the map layout
     */
    virtual void GenerateAttributes();

    /**
     * @brief Generate coordinates of an entrance in the given direction
     * 
     * @param dir direction
     * @return Coords entrance coords
     */
    Coords GenerateEntranceCoords(Direction dir);

    /**
     * @brief Draw a line of fields at and between the two given positions in the same line
     * 
     * @param from first position
     * @param to second position
     * @param value value to apply (default: true)
     */
    void DrawMapLine(Coords from, Coords to, bool value = true);

    /**
     * @brief Draw a box of fields centered at the given coords
     * 
     * @param center center
     * @param radius distance from the center to the edge
     * @param value value to apply (default: true)
     */
    void DrawMapBox(Coords center, size_t radius, bool value = true);
};

} /* namespace Generation */

} /* namespace Worlds */
#pragma once

#include "Field.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "RoomGenerationParameters.h"
#include "UI/CameraStyle.h"
#include <cstdint>
#include <map>
#include <vector>

namespace Worlds::Generation
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
     * @brief How many layout types there are
     */
    constexpr static const int NumberOfTypes = 2;

    /**
     * @brief Radius around an entrance field which should be kept empty
     */
    constexpr static const int EntranceRadius = 1;

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
     * @return UI::CameraStyle camera style
     */
    UI::CameraStyle GetCameraStyle() const;

    /**
     * @brief Get the vision radius
     * 
     * @return int vision radius
     */
    int GetVisionRadius() const;

protected:
    /**
     * @brief Type of field for generation
     */
    enum class FieldType : std::int8_t
    {
        /**
         * @brief Empty inaccessible field
         */
        Inaccessible,

        /**
         * @brief Empty accessible field
         */
        Accessible,

        /**
         * @brief Wall
         */
        Wall,

        /**
         * @brief Column
         */
        Column
    };

    Coords::Scalar m_Width;
    Coords::Scalar m_Height;
    std::vector<std::vector<FieldType>> m_Map;
    const RoomGenerationParameters& m_Parameters;
    std::map<Direction, Coords> m_Entrances;
    UI::CameraStyle m_CameraStyle;
    int m_VisionRadius;

    /**
     * @brief Constructor
     * 
     * @param parameters parameters
     */
    RoomLayout(const RoomGenerationParameters& parameters);

    /**
     * @brief Generate the layout
     */
    virtual void Generate() = 0;

    /**
     * @brief Generate room attributes not related to the map layout
     */
    virtual void GenerateAttributes();

    /**
     * @brief Generate the directions to create entrances in
     * Takes into account EntranceInfo and other Parameters.
     * 
     * @return std::vector<Direction> list of directions
     */
    virtual std::vector<Direction> GenerateEntranceDirections() const;

    /**
     * @brief Generate coordinates of an entrance in the given direction
     * 
     * @param dir direction
     * @return Coords entrance coords
     */
    virtual Coords GenerateEntranceCoords(Direction dir) const;

    /**
     * @brief Draw a line of fields at and between the two given positions in the same line
     * 
     * @param from first position
     * @param to second position
     * @param value value to apply (default: wall)
     */
    void DrawMapLine(Coords from, Coords to, FieldType value = FieldType::Wall);

    /**
     * @brief Draw a box of fields centered at the given coords
     * 
     * @param center center
     * @param radius distance from the center to the edge
     * @param value value to apply (default: wall)
     */
    void DrawMapBox(Coords center, Coords::Scalar radius, FieldType value = FieldType::Wall);
};

} /* namespace Worlds::Generation */

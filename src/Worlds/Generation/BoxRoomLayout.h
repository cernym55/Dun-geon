#pragma once

#include "RoomGenerationParameters.h"
#include "RoomLayout.h"

namespace Worlds::Generation
{

/**
 * @brief Rectangular layout with straight walls and up to 4 entrances
 */
class BoxRoomLayout : public RoomLayout
{
public:
    /**
     * @brief Constructor
     * 
     * @param parameters parameters
     */
    BoxRoomLayout(const RoomGenerationParameters& parameters);

private:
    /**
     * @brief Determines the inside layout of the room
     */
    enum class Subtype
    {
        /**
         * @brief Empty room
         */
        Default = 0,

        /**
         * @brief Columns spawn with randomness
         */
        ColumnsOnly = 1
    };

    constexpr static const Coords::Scalar MaximumWidth = 30;
    constexpr static const Coords::Scalar MaximumHeight = 16;
    constexpr static const Coords::Scalar MinimumWidth = 20;
    constexpr static const Coords::Scalar MinimumHeight = 12;
    constexpr static const Coords::Scalar DarknessVisionRadius = 5;

    Subtype m_Subtype;

    /**
     * @brief Generate the layout
     */
    void Generate() override final;

    /**
     * @brief Generate room attributes not related to the map layout
     */
    void GenerateAttributes() override final;

    /**
     * @brief Used to generate columns for appropriate subtypes
     */
    void GenerateColumns();
};

} /* namespace Worlds::Generation */


#pragma once

#include "RoomGenerationParameters.h"
#include "RoomLayout.h"

namespace Worlds
{

namespace Generation
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

    BoxRoomLayout(const BoxRoomLayout&) = delete;

    BoxRoomLayout(BoxRoomLayout&&) = delete;

    BoxRoomLayout& operator=(const BoxRoomLayout&) = delete;

    BoxRoomLayout& operator=(BoxRoomLayout&&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~BoxRoomLayout() = default;

private:
    constexpr static const Coords::Scalar MaximumWidth = 30;
    constexpr static const Coords::Scalar MaximumHeight = 16;
    constexpr static const Coords::Scalar MinimumWidth = 20;
    constexpr static const Coords::Scalar MinimumHeight = 12;
    constexpr static const Coords::Scalar DarknessVisionRadius = 5;

    /**
     * @brief Generate the layout
     */
    void Generate() override final;

    /**
     * @brief Generate room attributes not related to the map layout
     */
    void GenerateAttributes() override final;
};

} /* namespace Generation */

} /* namespace Worlds */

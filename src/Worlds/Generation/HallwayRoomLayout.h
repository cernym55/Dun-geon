#pragma once

#include "RoomGenerationParameters.h"
#include "RoomLayout.h"

namespace Worlds
{

namespace Generation
{

/**
 * @brief Hallway leading from one entrance to another, can branch
 */
class HallwayRoomLayout : public RoomLayout
{
public:
    /**
     * @brief Constructor
     * 
     * @param parameters parameters
     */
    HallwayRoomLayout(const RoomGenerationParameters& parameters);

    HallwayRoomLayout(const HallwayRoomLayout&) = delete;

    HallwayRoomLayout(HallwayRoomLayout&&) = delete;

    HallwayRoomLayout& operator=(const HallwayRoomLayout&) = delete;

    HallwayRoomLayout& operator=(HallwayRoomLayout&&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~HallwayRoomLayout() = default;

private:
    constexpr static const size_t MaximumWidth = 30;
    constexpr static const size_t MaximumHeight = 18;
    constexpr static const size_t MinimumWidth = 24;
    constexpr static const size_t MinimumHeight = 16;
    constexpr static const size_t HallwayWidth = 3;
    constexpr static const size_t MinimumCornerDistance = 2;
    constexpr static const int DarknessVisionRadius = 4;

    /**
     * @brief Generate the layout
     */
    void Generate() override;

    /**
     * @brief Generate room attributes not related to the map layout
     */
    void GenerateAttributes() override;
};

} /* namespace Generation */

} /* namespace Worlds */
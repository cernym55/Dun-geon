#pragma once

#include "Misc/Direction.h"
#include "RoomLayout.h"
#include <map>

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
     * @param entranceInfo entrance info
     */
    BoxRoomLayout(const std::map<Direction, bool>& entranceInfo);

    BoxRoomLayout(const BoxRoomLayout&) = delete;

    BoxRoomLayout(BoxRoomLayout&&) = delete;

    BoxRoomLayout& operator=(const BoxRoomLayout&) = delete;

    BoxRoomLayout& operator=(BoxRoomLayout&&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~BoxRoomLayout() = default;

private:
    constexpr static const size_t MaximumWidth = 30;
    constexpr static const size_t MaximumHeight = 16;
    constexpr static const size_t MinimumWidth = 20;
    constexpr static const size_t MinimumHeight = 12;

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
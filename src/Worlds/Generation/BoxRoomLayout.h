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

protected:
    constexpr static const size_t MaximumWidth = 40;
    constexpr static const size_t MaximumHeight = 20;
    constexpr static const size_t MinimumWidth = 20;
    constexpr static const size_t MinimumHeight = 12;

    /**
     * @brief Generate the layout
     */
    virtual void Generate() override;

private:
    /**
     * @brief Generate coordinates of an entrance in the given direction
     * 
     * @param dir direction
     * @return Coords entrance coords
     */
    Coords GenerateEntranceCoords(Direction dir);
};

} /* namespace Generation */

} /* namespace Worlds */
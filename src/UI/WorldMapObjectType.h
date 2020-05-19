#pragma once

namespace UI
{

/**
 * @brief Types of objects visible and selectable on the world map
 * 
 */
enum class WorldMapObjectType
{
    /**
     * @brief Empty
     */
    Empty = 0,

    /**
     * @brief Room
     */
    Room,

    /**
     * @brief Undiscovered room
     */
    UndiscoveredRoom
};

} /* namespace UI */
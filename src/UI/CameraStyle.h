#pragma once

namespace UI
{

/**
 * @brief Camera style used in the world window
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

} /* namespace UI */
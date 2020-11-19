#pragma once

#include "Character.h"
#include "Misc/Direction.h"
#include <ncurses.h>
#include <string>

namespace Entities
{

/**
 * @brief Player character class
 */
class Player : public Character
{
public:
    /**
     * @brief Constructor
     *
     * @param name name
     * @param icon icon (default: set to first character of name)
     */
    Player(const std::string& name,
           chtype icon = 0);

    /**
     * @brief Set the direction of the last move
     *
     * @param dir direction
     */
    void SetLastMoveDirection(Direction dir);

    /**
     * @brief Set the coordinates
     *
     * @param coords coords
     */
    void SetCoords(Coords coords);
};

} /* namespace Entities */

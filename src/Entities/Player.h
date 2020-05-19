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
    Player(const std::string& name,
           chtype icon = 0);

    void SetLastMoveDirection(Direction dir);

    void SetCoords(Coords coords);
};

} /* namespace Entities */

#include "Player.h"
#include "Misc/Direction.h"
#include <ncurses.h>

namespace Entities
{

/**
 * @brief Constructor
 *
 * @param name name
 * @param icon icon (default: set to first character of name)
 */
Player::Player(const std::string& name,
               chtype icon)
    : Character(name, "", icon, true)
{
}

/**
 * @brief Set the direction of the last move
 *
 * @param dir direction
 */
void Player::SetLastMoveDirection(Direction dir)
{
    m_LastMoveDirection = dir;
}

/**
 * @brief Set the coordinates
 *
 * @param coords coords
 */
void Player::SetCoords(Coords coords)
{
    m_Coords = coords;
}

} /* namespace Entities */

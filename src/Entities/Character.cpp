#include "Character.h"
#include "Misc/Direction.h"
#include "Worlds/Room.h"
#include <ncurses.h>
#include <string>

namespace Entities
{

/**
 * @brief Constructor
 *
 * @param name name
 * @param description description (default: empty)
 * @param icon icon (default: set to first character of name)
 * @param isBlocking blocking attribute (default: true)
 */
Character::Character(const std::string& name,
                     const std::string& description,
                     chtype icon,
                     bool isBlocking)
    : Entity(name, description, icon, isBlocking),
      m_LastMoveDirection(Direction::None())
{
}

/**
 * @brief Move the character position in the given direction
 *
 * @param dir direction
 */
void Character::Move(Direction dir)
{
    m_Coords.MoveInDirection(dir);
    m_LastMoveDirection = dir;
}

/**
 * @brief Get the direction of the last move the character performed
 *
 * @return Direction last move direction
 */
Direction Character::GetLastMoveDirection() const
{
    return m_LastMoveDirection;
}

Stats& Character::GetStats()
{
    return m_Stats;
}

const Stats& Character::GetStats() const
{
    return m_Stats;
}

} /* namespace Entities */

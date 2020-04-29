#include "Character.h"
#include "Misc/Direction.h"
#include "Worlds/Room.h"
#include <ncurses.h>
#include <string>

namespace Entities
{

Character::Character(const std::string& name,
                     const std::string& description,
                     chtype icon,
                     bool isBlocking)
    : Entity(name, description, icon, isBlocking),
      m_LastMoveDirection(Direction::None())
{
}

void Character::Move(Direction dir)
{
    m_Coords.MoveInDirection(dir);
    m_LastMoveDirection = dir;
}

Stats& Character::GetStats()
{
    return m_Stats;
}

const Stats& Character::GetStats() const
{
    return m_Stats;
}

Direction Character::GetLastMoveDirection() const
{
    return m_LastMoveDirection;
}

} /* namespace Entities */

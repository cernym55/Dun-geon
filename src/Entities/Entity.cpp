#include "Entity.h"
#include "EntityManager.h"
#include "Misc/Coords.h"
#include <ncurses.h>

namespace Entities
{

Entity::Entity(const std::string& name,
               const std::string& description,
               chtype icon,
               bool isBlocking)
    : m_Name(name),
      m_Description(description),
      m_Icon(icon != 0 ? icon : name[0]),
      m_Blocking(isBlocking)
{
}

Direction Entity::GetNextMove(const EntityManager& entityManager)
{
    return Direction::None;
}

bool Entity::Fightable() const
{
    return false;
}

bool Entity::IsBlocking() const
{
    return m_Blocking;
}

chtype Entity::GetIcon() const
{
    return m_Icon;
}

const std::string& Entity::GetName() const
{
    return m_Name;
}

const std::string& Entity::GetDescription() const
{
    return m_Description;
}

} /* namespace Entities */

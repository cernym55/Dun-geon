#include "Entity.h"
#include "Misc/Coords.h"

namespace Entities
{

Entity::Entity(const std::string& name,
               const std::string& description,
               char icon,
               bool isBlocking)
    : m_Name(name),
      m_Description(description),
      m_Icon(icon != 0 ? icon : name[0]),
      m_Blocking(isBlocking)
{
}

void Entity::SetCoords(Coords value)
{
    m_Coords = value;
}

Coords Entity::GetCoords() const
{
    return m_Coords;
}

bool Entity::IsBlocking() const
{
    return m_Blocking;
}

char Entity::GetIcon() const
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

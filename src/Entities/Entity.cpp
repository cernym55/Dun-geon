#include "Entity.h"
#include "Misc/Coords.h"
#include <ncurses.h>

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

/**
 * @brief Get the coordinates
 *
 * @return coordinates
 */
Coords Entity::GetCoords() const
{
    return m_Coords;
}

/**
 * @brief Check if this entity is blocking (can't overlap with other entities)
 *
 * @return true if blocking
 */
bool Entity::IsBlocking() const
{
    return m_Blocking;
}

/**
 * @brief Get the icon
 *
 * @return char icon
 */
chtype Entity::GetIcon() const
{
    return m_Icon;
}

/**
 * @brief Get the name
 *
 * @return const std::string& name
 */
const std::string& Entity::GetName() const
{
    return m_Name;
}

/**
 * @brief Get the description
 *
 * @return const std::string& description
 */
const std::string& Entity::GetDescription() const
{
    return m_Description;
}

} /* namespace Entities */

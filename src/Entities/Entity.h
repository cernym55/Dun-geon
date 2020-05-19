#pragma once

#include "Misc/Coords.h"
#include <iostream>
#include <ncurses.h>
#include <string>

namespace Entities
{

/**
 * @brief An object or character occupying fields on the game map
 */
class Entity
{
public:
    Entity(const std::string& name,
           const std::string& description = "",
           chtype icon = 0,
           bool isBlocking = true);

    Coords GetCoords() const;

    bool IsBlocking() const;

    chtype GetIcon() const;

    const std::string& GetName() const;

    const std::string& GetDescription() const;

protected:
    std::string m_Name;
    std::string m_Description;
    chtype m_Icon;
    bool m_Blocking;
    Coords m_Coords;
};

} /* namespace Entities */

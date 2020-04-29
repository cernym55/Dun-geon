#pragma once

#include "Character.h"
#include "Misc/Direction.h"
#include <ncurses.h>
#include <string>

namespace Entities
{

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
    
    Player(const Player&) = delete;

    Player(Player&&) = delete;

    Player& operator=(const Player&) = delete;

    Player& operator=(Player&&) = delete;

    /**
     * @brief Destructor
     */
    virtual ~Player() = default;

    /**
     * @brief Set the direction of the last move
     * 
     * @param dir direction
     */
    void SetLastMoveDirection(Direction dir);

private:
};

} /* namespace Entities */
#pragma once

#include "character.h"

class Player : public Character
{
private:
public:
    bool attemptMove(Direction dir);
};

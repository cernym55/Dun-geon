#pragma once

#include "character.h"

namespace Entities
{

class Player : public Character
{
private:
public:
    bool attemptMove(Direction dir);
};

} /* namespace Entities */
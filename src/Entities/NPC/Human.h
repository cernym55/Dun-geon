#pragma once

#include "NPCCharacter.h"

namespace Entities::NPC
{

/**
 * @brief Human character
 */
class Human : public NPCCharacter
{
public:
    /**
     * @brief Constructor
     * 
     * @param initialCoords initial coords
     * @param name name
     */
    Human(Coords initialCoords, const std::string& name);
};

} /* namespace Entities::NPC */
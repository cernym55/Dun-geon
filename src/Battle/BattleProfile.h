#pragma once

#include "Entities/Character.h"

namespace Battle
{

/**
 * @brief Stores information relevant to Battles for a character
 */
class BattleProfile
{
public:
    Entities::Character::Stats Stats;

    /**
     * @brief Constructor
     * 
     * @param stats character stats
     */
    BattleProfile(const Entities::Character::Stats& stats);

private:
};

} /* namespace Battle */
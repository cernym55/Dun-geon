#pragma once

#include "Entities/Stats.h"

namespace Battle
{

/**
 * @brief Stores information relevant to Battles for a character
 */
class BattleProfile
{
public:
    Entities::Stats Stats;

    /**
     * @brief Constructor
     * 
     * @param stats character stats
     */
    BattleProfile(const Entities::Stats& stats);

private:
};

} /* namespace Battle */
#pragma once

#include "Entities/Stats.h"
#include "Effect.h"
#include <memory>
#include <vector>

namespace Battle
{

/**
 * @brief Stores information relevant to Battles for a character
 */
class BattleProfile
{
public:
    Entities::Stats Stats;

    struct
    {
        int Physical;
        // TODO: Add more
    } Resistances;

    std::vector<std::unique_ptr<Effect>> ActiveEffects;

    /**
     * @brief Constructor
     *
     * @param stats character stats
     */
    BattleProfile(const Entities::Stats& stats);

private:
};

} /* namespace Battle */
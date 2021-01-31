#pragma once

#include "Entities/Stats.h"
#include "Effect.h"
#include <array>
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

    std::array<int, 5> Resistances;

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
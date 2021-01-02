#pragma once

#include "BattleProfile.h"
#include "Effect.h"

namespace Battle::EffectCollection
{

/**
 * @brief Grants 50% physical damage reduction for a single turn
 */
class Brace : public Effect
{
    Brace(BattleProfile& user, BattleProfile& target, int duration) : Effect(user, target, duration) {}

    void Apply() override { m_Target.Resistances.Physical += 50; }

    void Remove() override { m_Target.Resistances.Physical -= 50; }

    void TickAction() override
    {
        // do nothing
    }
};

} /* namespace Battle::EffectCollection */
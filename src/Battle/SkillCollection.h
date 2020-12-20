#pragma once

#include "AttackSkill.h"

namespace Battle::SkillCollection
{

/**
 * @brief Swing
 */
class Swing : public AttackSkill
{
public:
    Swing()
        : AttackSkill(Category::Melee,
                      "Swing",
                      "Take a swing at your foe",
                      "Reliable basic melee attack.",
                      { 2, 4 },
                      90,
                      8,
                      0)
    {
    }

    inline int CalculateEffectiveDamage(int baseDamage,
                                        const BattleProfile& userProfile,
                                        const BattleProfile& targetProfile) const override
    {
        return baseDamage + userProfile.Stats.Strength / 10;
    }
};

/**
 * @brief Weak magic attack
 */
class Wail : public AttackSkill
{
public:
    Wail() : AttackSkill(Category::Spell, "Wail", "", "", { 2, 3 }, 80, 5, 0) {}

    inline int CalculateEffectiveDamage(int baseDamage,
                                        const BattleProfile& userProfile,
                                        const BattleProfile& targetProfile) const override
    {
        return baseDamage + userProfile.Stats.Sorcery / 10;
    }
};

} /* namespace Battle::SkillCollection */
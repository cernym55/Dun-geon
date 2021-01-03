#pragma once

#include "ApplyEffectOnlySkill.h"
#include "AttackSkill.h"
#include "EffectCollection.h"

namespace Battle::SkillCollection
{

class Brace : public ApplyEffectOnlySkill<EffectCollection::Brace>
{
public:
    Brace()
        : ApplyEffectOnlySkill(Category::Special,
                               Target::Self,
                               "Brace",
                               "Brace for the next blow",
                               "Increases physical resistance by 50 % for the next turn.",
                               100,
                               0)
    {
    }
};

/**
 * @brief Weak melee attack with high Crit chance and no damage scaling (NPC only)
 */
class Nibble : public AttackSkill
{
public:
    Nibble() : AttackSkill(Category::Melee, "Nibble", "", "", { 1, 3 }, 90, 15, 0) {}

    inline int CalculateEffectiveDamage(int baseDamage,
                                        const BattleProfile& userProfile,
                                        const BattleProfile& targetProfile) const override
    {
        return baseDamage;
    }
};

/**
 * @brief Basic melee attack
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
 * @brief Basic magic attack (NPC only)
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
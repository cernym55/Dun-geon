#include "Skill.h"
#include "Misc/RNG.h"

namespace Battle
{

Skill::Skill(Skill::Category category,
             const std::string& name,
             const std::string& flavorText,
             const std::string& longDescription,
             const std::pair<int, int>& baseDamageRange,
             int baseHitChance,
             int baseCritChance,
             int baseManaCost,
             std::function<int(int, const BattleProfile&, const BattleProfile&)> damageFormula)
    : m_Category(category),
      m_Name(name),
      m_FlavorText(flavorText),
      m_LongDescription(longDescription),
      m_BaseDamageRange(baseDamageRange),
      m_BaseHitChance(baseHitChance),
      m_BaseCritChance(baseCritChance),
      m_BaseManaCost(baseManaCost),
      m_DamageFormula(damageFormula)
{
}

Skill::ApplySkillResult Skill::ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile)
{
    bool hit = RNG::Chance(CalculateHitChance(userProfile, targetProfile) / 100.);
    if (!hit)
        return { false, false, 0 };

    auto damageRange = CalculateEffectiveDamageRange(userProfile, targetProfile);
    int damage       = RNG::RandomInt(damageRange.first, damageRange.second + 1);

    bool crit = RNG::Chance(CalculateCritChance(userProfile, targetProfile) / 100.);
    if (crit)
        damage *= 2;

    targetProfile.Stats.Health -= damage;

    return { true, crit, damage };
}

int Skill::GetManaCost() const
{
    return m_BaseManaCost;
}

std::pair<int, int> Skill::CalculateEffectiveDamageRange(const BattleProfile& userProfile,
                                                         const BattleProfile& targetProfile) const
{
    return { CalculateEffectiveDamage(m_BaseDamageRange.first, userProfile, targetProfile),
             CalculateEffectiveDamage(m_BaseDamageRange.second, userProfile, targetProfile) };
}

int Skill::CalculateHitChance(const BattleProfile& userProfile, const BattleProfile& targetProfile) const
{
    auto levelDifference = targetProfile.Stats.Level - userProfile.Stats.Level;
    return m_BaseHitChance - levelDifference * 2 + userProfile.Stats.Dexterity / 4.0;
}

int Skill::CalculateCritChance(const BattleProfile& userProfile, const BattleProfile& targetProfile) const
{
    return m_BaseCritChance;
}

int Skill::CalculateEffectiveDamage(int baseDamage,
                                    const BattleProfile& userProfile,
                                    const BattleProfile& targetProfile) const
{
    return m_DamageFormula(baseDamage, userProfile, targetProfile);
}

} /* namespace Battle */
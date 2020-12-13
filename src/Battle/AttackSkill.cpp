#include "AttackSkill.h"
#include "UI/BattleScreen.h"
#include "Misc/RNG.h"

namespace Battle
{

AttackSkill::AttackSkill(Category category,
                         const std::string& name,
                         const std::string& flavorText,
                         const std::string& longDescription,
                         const std::pair<int, int>& baseDamageRange,
                         int baseHitChance,
                         int baseCritChance,
                         int baseManaCost,
                         std::function<int(int, const BattleProfile&, const BattleProfile&)> damageFormula)
    : Skill(category, name, flavorText, longDescription, baseManaCost),
      m_BaseDamageRange(baseDamageRange),
      m_BaseHitChance(baseHitChance),
      m_BaseCritChance(baseCritChance),
      m_DamageFormula(damageFormula)
{
}

Skill::ApplySkillResult AttackSkill::ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile)
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

void AttackSkill::OnBattleMenuHover(UI::BattleScreen& battleScreen)
{
    battleScreen.ProjectSkillUse(*this);
    battleScreen.PrintSkillHoverThumbnailInfo(*this);
}

std::pair<int, int> AttackSkill::CalculateEffectiveDamageRange(const BattleProfile& userProfile,
                                                               const BattleProfile& targetProfile) const
{
    return { CalculateEffectiveDamage(m_BaseDamageRange.first, userProfile, targetProfile),
             CalculateEffectiveDamage(m_BaseDamageRange.second, userProfile, targetProfile) };
}

int AttackSkill::CalculateHitChance(const BattleProfile& userProfile, const BattleProfile& targetProfile) const
{
    auto levelDifference = targetProfile.Stats.Level - userProfile.Stats.Level;
    return m_BaseHitChance - levelDifference * 2 + userProfile.Stats.Dexterity / 4.0;
}

int AttackSkill::CalculateCritChance(const BattleProfile& userProfile, const BattleProfile& targetProfile) const
{
    return m_BaseCritChance;
}

int AttackSkill::CalculateEffectiveDamage(int baseDamage,
                                          const BattleProfile& userProfile,
                                          const BattleProfile& targetProfile) const
{
    return m_DamageFormula(baseDamage, userProfile, targetProfile);
}

} /* namespace Battle */
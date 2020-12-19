#include "AttackSkill.h"
#include "Misc/RNG.h"
#include "UI/BattleScreen.h"
#include <algorithm>
#include <cmath>

namespace Battle
{

AttackSkill::AttackSkill(Category category,
                         const std::string& name,
                         const std::string& flavorText,
                         const std::string& longDescription,
                         const std::pair<int, int>& baseDamageRange,
                         int baseHitChance,
                         int baseCritChance,
                         int baseManaCost)
    : Skill(category, name, flavorText, longDescription, baseManaCost),
      m_BaseDamageRange(baseDamageRange),
      m_BaseHitChance(baseHitChance),
      m_BaseCritChance(baseCritChance)
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
        damage *= 1.4;

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
    int levelDifference      = targetProfile.Stats.Level - userProfile.Stats.Level;
    double targetDodgeChance = (levelDifference > 0 ? levelDifference * 2.5 : 0) + targetProfile.Stats.Dexterity / 7.0;
    return lround(std::clamp(
        (m_BaseHitChance + userProfile.Stats.Dexterity / 4.0) * (100 - targetDodgeChance) / 100.0, 0.0, 100.0));
}

int AttackSkill::CalculateCritChance(const BattleProfile& userProfile, const BattleProfile& targetProfile) const
{
    int levelDifferenceFactor = (targetProfile.Stats.Level - userProfile.Stats.Level) * 0.5;
    if (m_Category == Category::Spell)
    {
        return lround(
            std::clamp(m_BaseCritChance + 0.1 * userProfile.Stats.Sorcery + levelDifferenceFactor, 0.0, 100.0));
    }
    return lround(
        std::clamp(m_BaseCritChance + 0.05 * userProfile.Stats.Dexterity + levelDifferenceFactor, 0.0, 100.0));
}

} /* namespace Battle */
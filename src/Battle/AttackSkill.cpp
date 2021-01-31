#include "AttackSkill.h"
#include "Misc/Exceptions.h"
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
                         DamageType damageType,
                         int baseHitChance,
                         int baseCritChance,
                         int baseManaCost)
    : Skill(category, Target::Opponent, name, flavorText, longDescription, baseManaCost),
      m_BaseDamageRange(baseDamageRange),
      m_DamageType(damageType),
      m_BaseHitChance(baseHitChance),
      m_BaseCritChance(baseCritChance)
{
}

void AttackSkill::ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile)
{
    m_LastApplyResult.reset();
    bool hit = RNG::Chance(CalculateHitChance(userProfile, targetProfile) / 100.);
    if (!hit)
    {
        m_LastApplyResult = std::make_unique<AttackSkillResult>(
            AttackSkillResult { false, false, DamageInstance { 0, m_DamageType } });
        return;
    }

    auto damageRange = CalculateEffectiveDamageRange(userProfile, targetProfile);
    int damage       = RNG::RandomInt(damageRange.first, damageRange.second + 1);

    bool crit = RNG::Chance(CalculateCritChance(userProfile, targetProfile) / 100.);
    if (crit)
    {
        // Guarantee damage will be at least the top end of the damage range, or add 40 %
        damage = std::max(static_cast<int>(damage * 1.4), damageRange.second);
    }

    // Damage modifiers
    damage *= (100 - targetProfile.Resistances[m_DamageType.ToInt()]) / 100.0;

    targetProfile.Stats.Health -= damage;

    m_LastApplyResult = std::make_unique<AttackSkillResult>(
        AttackSkillResult { true, crit, DamageInstance { damage, m_DamageType } });
}

void AttackSkill::OnBattleMenuHover(UI::BattleScreen& battleScreen)
{
    battleScreen.ProjectSkillUse(*this);
    battleScreen.PrintSkillHoverThumbnailInfo(*this);
}

void AttackSkill::AnimateTo(UI::BattleScreen& battleScreen, bool isPlayer) const
{
    if (!m_LastApplyResult)
    {
        throw CustomException("AttackSkill::AnimateTo() failed - no last usage record");
    }

    if (isPlayer)
    {
        battleScreen.AnimatePlayerAttack(*m_LastApplyResult);
    }
    else
    {
        battleScreen.AnimateEnemyAttack(*m_LastApplyResult, m_Name);
    }
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
    double levelDifferenceFactor = (userProfile.Stats.Level - targetProfile.Stats.Level) * 0.5;
    if (m_Category == Category::Spell)
    {
        return lround(
            std::clamp(m_BaseCritChance + 0.1 * userProfile.Stats.Sorcery + levelDifferenceFactor, 0.0, 100.0));
    }
    return lround(
        std::clamp(m_BaseCritChance + 0.05 * userProfile.Stats.Dexterity + levelDifferenceFactor, 0.0, 100.0));
}

} /* namespace Battle */
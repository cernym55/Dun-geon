#pragma once

#include "Skill.h"

namespace Battle
{

class AttackSkill : public Skill
{
public:
    /**
     * @brief Constructor
     *
     * @param category skill category
     * @param name skill name
     * @param flavorText flavor text (up to 24 characters)
     * @param longDescription longer description
     * @param baseDamageRange base damage range
     * @param baseHitChance base hit chance
     * @param baseCritChance base crit chance
     * @param baseManaCost base mana cost
     */
    AttackSkill(Category category,
                const std::string& name,
                const std::string& flavorText,
                const std::string& longDescription,
                const std::pair<int, int>& baseDamageRange,
                int baseHitChance,
                int baseCritChance,
                int baseManaCost);

    /**
     * @brief Destructor
     */
    virtual ~AttackSkill() = default;

    /**
     * @brief Apply skill to target
     *
     * @param userProfile user battle profile
     * @param targetProfile target battle profile to apply effects to
     * @return ApplySkillResult result
     */
    virtual ApplySkillResult ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile) override;

    /**
     * @brief Send data to the battle screen to draw the hover thumbnail
     *
     * @param battleScreen battle screen
     */
    virtual void OnBattleMenuHover(UI::BattleScreen& battleScreen) override;

    /**
     * @brief Calculate the effective damage dealt for a particular instance
     *
     * @param baseDamage base damage figure
     * @param userProfile attack user profile
     * @param targetProfile attack target profile
     * @return int effective damage
     */
    virtual int CalculateEffectiveDamage(int baseDamage,
                                         const BattleProfile& userProfile,
                                         const BattleProfile& targetProfile) const = 0;

    /**
     * @brief Get the effective damage range
     *
     * @param userProfile skill user profile
     * @param targetProfile skill target profile
     * @return std::pair<int, int> effective damage range
     */
    std::pair<int, int> CalculateEffectiveDamageRange(const BattleProfile& userProfile,
                                                      const BattleProfile& targetProfile) const;

    /**
     * @brief Calculate effective hit chance for a particular instance
     *
     * @param userProfile skill user profile
     * @param targetProfile skill target profile
     * @return int effective hit chance
     */
    int CalculateHitChance(const BattleProfile& userProfile, const BattleProfile& targetProfile) const;

    /**
     * @brief Calculate effective crit chance for a particular instance
     *
     * @param userProfile skill user profile
     * @param targetProfile skill target profile
     * @return int effective crit chance
     */
    int CalculateCritChance(const BattleProfile& userProfile, const BattleProfile& targetProfile) const;

protected:
    std::pair<int, int> m_BaseDamageRange;
    int m_BaseHitChance;
    int m_BaseCritChance;
};

} /* namespace Battle */
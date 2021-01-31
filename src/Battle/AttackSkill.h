#pragma once

#include "DamageInstance.h"
#include "DamageType.h"
#include "Skill.h"

namespace Battle
{

class AttackSkill : public Skill
{
public:
    /**
     * @brief Attack skill result data
     */
    struct AttackSkillResult : public SkillResult
    {
        /**
         * @brief Was a crit (if applicable)
         */
        bool IsCrit;

        /**
         * @brief Damage value (if applicable)
         */
        DamageInstance Damage;
    };

    /**
     * @brief Constructor
     *
     * @param category skill category
     * @param name skill name
     * @param flavorText flavor text (up to 24 characters)
     * @param longDescription longer description
     * @param baseDamageRange base damage range
     * @param damageType damage type
     * @param baseHitChance base hit chance
     * @param baseCritChance base crit chance
     * @param baseManaCost base mana cost
     */
    AttackSkill(Category category,
                const std::string& name,
                const std::string& flavorText,
                const std::string& longDescription,
                const std::pair<int, int>& baseDamageRange,
                DamageType damageType,
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
     * @return SkillResult result
     */
    virtual void ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile) override;

    /**
     * @brief Send data to the battle screen to draw the hover thumbnail
     *
     * @param battleScreen battle screen
     */
    virtual void OnBattleMenuHover(UI::BattleScreen& battleScreen) override;

    /**
     * @brief Animate the result of the last skill usage on the battle screen
     * 
     * @param battleScreen battle screen
     * @param isPlayer true if the user is the player
     */
    virtual void AnimateTo(UI::BattleScreen& battleScreen, bool isPlayer) const override;

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

    /**
     * @brief Get the damage type
     * 
     * @return DamageType damage type
     */
    inline DamageType GetDamageType() const { return m_DamageType; }

protected:
    std::pair<int, int> m_BaseDamageRange;
    DamageType m_DamageType;
    int m_BaseHitChance;
    int m_BaseCritChance;

private:
    std::unique_ptr<AttackSkillResult> m_LastApplyResult;
};

} /* namespace Battle */
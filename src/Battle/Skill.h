#pragma once

#include "BattleProfile.h"
#include <functional>
#include <string>

namespace Battle
{

/**
 * @brief Ability or passive effect used by characters
 */
class Skill
{
public:
    /**
     * @brief Category where the skill can be found in the battle menu
     */
    enum class Category
    {
        Passive,
        Melee,
        Ranged,
        Spell,
        Special
    };

    /**
     * @brief Result data for ApplySkill
     */
    struct ApplySkillResult
    {
        /**
         * @brief Was a hit
         */
        bool IsHit;
        /**
         * @brief Was a crit (if applicable)
         */
        bool IsCrit;

        /**
         * @brief Damage or healing value (if applicable)
         */
        int Value;
    };

    /**
     * @brief Constructor
     *
     * @param category category
     * @param name name
     * @param flavorText flavor text (max 24 characters)
     * @param longDescription longer description of what the skill does
     * @param baseDamageRange base damage range
     * @param baseHitChance base hit chance
     * @param baseCritChance base crit chance
     * @param baseManaCost base mana cost
     * @param damageFormula function to calculate damage dealt by attacker to target
     */
    Skill(Category category,
          const std::string& name,
          const std::string& flavorText,
          const std::string& longDescription,
          const std::pair<int, int>& baseDamageRange,
          int baseHitChance,
          int baseCritChance,
          int baseManaCost,
          std::function<int(int, const BattleProfile&, const BattleProfile&)> damageFormula);

    /**
     * @brief Apply skill to target
     *
     * @param userProfile user battle profile
     * @param targetProfile target battle profile to apply effects to
     * @return ApplySkillResult result
     */
    virtual ApplySkillResult ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile);

    /**
     * @brief Get the Category
     *
     * @return Category category
     */
    inline Category GetCategory() const { return m_Category; }

    /**
     * @brief Get the Name
     *
     * @return const std::string& name
     */
    inline const std::string& GetName() const { return m_Name; }

    /**
     * @brief Get the Flavor Text
     *
     * @return const std::string& flavor text
     */
    inline const std::string& GetFlavorText() const { return m_FlavorText; }

    /**
     * @brief Get the Long Description
     *
     * @return const std::string& long description
     */
    inline const std::string& GetLongDescription() const { return m_LongDescription; }

    /**
     * @brief Get the mana cost
     * 
     * @return int mana cost
     */
    int GetManaCost() const;

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
     * @brief Calculate the effective damage dealt for a particular instance
     *
     * @param baseDamage base damage figure
     * @param userProfile attack user profile
     * @param targetProfile attack target profile
     * @return int effective damage
     */
    int CalculateEffectiveDamage(int baseDamage,
                                 const BattleProfile& userProfile,
                                 const BattleProfile& targetProfile) const;

private:
    Category m_Category;
    std::string m_Name;
    std::string m_FlavorText; // max 24 characters
    std::string m_LongDescription;
    std::pair<int, int> m_BaseDamageRange;
    int m_BaseHitChance;
    int m_BaseCritChance;
    int m_BaseManaCost;
    std::function<int(int, const BattleProfile&, const BattleProfile&)> m_DamageFormula;
};

} /* namespace Battle */
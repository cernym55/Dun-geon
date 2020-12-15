#pragma once

#include "BattleProfile.h"
#include <string>

namespace UI
{
class BattleScreen;
}

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
     * @param category skill category
     * @param name skill name
     * @param flavorText flavor text (up to 24 characters)
     * @param longDescription longer description
     * @param baseManaCost base mana cost
     */
    Skill(Category category,
          const std::string& name,
          const std::string& flavorText,
          const std::string& longDescription,
          int baseManaCost);

    /**
     * @brief Destructor
     */
    virtual ~Skill() = default;

    /**
     * @brief Apply skill to target
     *
     * @param userProfile user battle profile
     * @param targetProfile target battle profile to apply effects to
     * @return ApplySkillResult result
     */
    virtual ApplySkillResult ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile) = 0;

    /**
     * @brief Action to perform when the skill is hovered over in the battle menu
     *
     * @param battleScreen battle screen
     */
    virtual void OnBattleMenuHover(UI::BattleScreen& battleScreen) = 0;

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

private:
    Category m_Category;
    std::string m_Name;
    std::string m_FlavorText;
    std::string m_LongDescription;
    int m_BaseManaCost;
};

} /* namespace Battle */
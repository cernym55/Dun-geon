#pragma once

#include "Effect.h"
#include "Misc/RNG.h"
#include "Skill.h"
#include <memory>

namespace Battle
{

template<typename EffectType> class ApplyEffectOnlySkill : public Skill
{
public:
    /**
     * @brief Constructor
     *
     * @param category skill category
     * @param targetType skill target type
     * @param name skill name
     * @param flavorText flavor text (up to 24 characters)
     * @param longDescription longer description
     * @param baseHitChance base hit chance
     * @param baseDuration base duration
     * @param baseManaCost base mana cost
     */
    ApplyEffectOnlySkill(Category category,
                         Target targetType,
                         const std::string& name,
                         const std::string& flavorText,
                         const std::string& longDescription,
                         int baseHitChance,
                         int baseDuration,
                         int baseManaCost)
        : Skill(category, targetType, name, flavorText, longDescription, baseManaCost),
          m_BaseHitChance(baseHitChance),
          m_BaseDuration(baseDuration)
    {
    }

    /**
     * @brief Destructor
     */
    virtual ~ApplyEffectOnlySkill() = default;

    /**
     * @brief Apply skill to target
     *
     * @param userProfile user battle profile
     * @param targetProfile target battle profile to apply effects to
     * @return ApplySkillResult result
     */
    virtual ApplySkillResult ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile) override
    {
        bool hit = RNG::Chance(CalculateHitChance(userProfile, targetProfile) / 100.);
        if (hit)
        {
            auto& effect
                = targetProfile.ActiveEffects.emplace_back(std::make_unique<EffectType>(userProfile, targetProfile, m_BaseDuration));
            effect->Apply();
        }
        return { hit, false, 0 };
    }

    /**
     * @brief Send data to the battle screen to draw the hover thumbnail
     *
     * @param battleScreen battle screen
     */
    virtual void OnBattleMenuHover(UI::BattleScreen& battleScreen) override
    {
        // TODO
    }

    /**
     * @brief Calculate effective hit chance for a particular instance
     *
     * @param userProfile skill user profile
     * @param targetProfile skill target profile
     * @return int effective hit chance
     */
    virtual int CalculateHitChance(const BattleProfile& userProfile, const BattleProfile& targetProfile) const
    {
        return m_BaseHitChance;
    }

protected:
    int m_BaseHitChance;
    int m_BaseDuration;
};

} /* namespace Battle */
#pragma once

#include "Effect.h"
#include "Misc/Exceptions.h"
#include "Misc/RNG.h"
#include "Skill.h"
#include <memory>

namespace UI
{
class BattleScreen;
}

namespace Battle
{

struct ApplyEffectOnlySkillResult : public Skill::SkillResult
{
};

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
     * @param effectDescription brief description of the effect
     * @param baseHitChance base hit chance
     * @param baseDuration base duration
     * @param baseManaCost base mana cost
     */
    ApplyEffectOnlySkill(Category category,
                         Target targetType,
                         const std::string& name,
                         const std::string& flavorText,
                         const std::string& longDescription,
                         const std::string& effectDescription,
                         int baseHitChance,
                         int baseDuration,
                         int baseManaCost)
        : Skill(category, targetType, name, flavorText, longDescription, baseManaCost),
          m_BaseHitChance(baseHitChance),
          m_BaseDuration(baseDuration),
          m_EffectDescription(effectDescription)
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
     */
    virtual void ApplySkill(const BattleProfile& userProfile, BattleProfile& targetProfile) override
    {
        m_LastApplyResult.reset();
        bool hit = RNG::Chance(CalculateHitChance(userProfile, targetProfile) / 100.);
        if (hit)
        {
            auto& effect = targetProfile.ActiveEffects.emplace_back(
                std::make_unique<EffectType>(userProfile, targetProfile, m_BaseDuration));
            effect->Apply();
        }

        m_LastApplyResult = std::make_unique<ApplyEffectOnlySkillResult>(ApplyEffectOnlySkillResult { hit });
    }

    /**
     * @brief Send data to the battle screen to draw the hover thumbnail
     *
     * @param battleScreen battle screen
     */
    virtual void OnBattleMenuHover(UI::BattleScreen& battleScreen) override;

    /**
     * @brief Get the SkillResult of the last skill usage
     *
     * @param battleScreen battle screen
     * @param isPlayer true if the user is the player
     */
    virtual void AnimateTo(UI::BattleScreen& battleScreen, bool isPlayer) const override;

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

    /**
     * @brief Get the Effect Description
     * 
     * @return const std::string& effect description
     */
    inline const std::string& GetEffectDescription() const { return m_EffectDescription; }

protected:
    int m_BaseHitChance;
    int m_BaseDuration;
    std::string m_EffectDescription;

private:
    std::unique_ptr<ApplyEffectOnlySkillResult> m_LastApplyResult;
};

} /* namespace Battle */
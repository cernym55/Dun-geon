#include "SkillCollection.h"
#include "Misc/RNG.h"

namespace Battle::SkillCollection
{

/**
 * @brief Swing
 */
Skill Swing(Battle::Skill::Category::Melee,
            "Swing",
            "Take a swing at your foe",
            "Reliable basic melee attack.",
            { 2, 4 },
            90,
            8,
            0,
            [](int baseDamage, const BattleProfile& userProfile, const BattleProfile& targetProfile) {
                return baseDamage + userProfile.Stats.Strength / 10;
            });

/**
 * @brief Punch
 */
Skill Punch(Battle::Skill::Category::Melee,
            "Punch",
            "",
            "",
            { 2, 3 },
            70,
            5,
            0,
            [](int baseDamage, const BattleProfile& userProfile, const BattleProfile& targetProfile) {
                return baseDamage + userProfile.Stats.Strength / 10;
            });

} /* namespace Battle::SkillCollection */
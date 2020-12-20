#pragma once

#include "Battle/SkillCollection.h"
#include "Character.h"
#include "UI/ColorPairs.h"
#include <cmath>

namespace Entities::NPC
{

/**
 * @brief Level 1-10 weak Apparition
 *
 */
class FadingSpirit : public Character
{
public:
    FadingSpirit(int level) : Character("Fading Spirit", "Apparition", 'y' | COLOR_PAIR(UI::ColorPairs::CyanOnDefault))
    {
        m_Stats = CalculateBaseStatsForLevel(level);
        GrantSkill<Battle::SkillCollection::Wail>();
    }

    Stats CalculateBaseStatsForLevel(int level) const override
    {
        Stats newStats;
        newStats.Level     = level;
        newStats.MaxHealth = lround(9 + (level - 1) * 1.7);
        newStats.Health    = newStats.MaxHealth;
        newStats.Strength  = 0;
        newStats.Dexterity = lround(10 + (level - 1) * 1.3);
        newStats.Sorcery   = lround(12 + (level - 1) * 1.5);
        newStats.Wisdom    = lround(8 + (level - 1) * 1.0);
        newStats.MaxMana   = newStats.Wisdom;
        newStats.Mana      = newStats.Wisdom;
        return newStats;
    }
};

} /* namespace Entities::NPC */
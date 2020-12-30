#pragma once

#include "Battle/SkillCollection.h"
#include "Character.h"
#include "UI/ColorPairs.h"
#include <cmath>

namespace Entities::NPCCollection
{

enum class Type
{
    FadingSpirit,
    Rat
};

/**
 * @brief Level 1-10 weak Apparition
 */
class FadingSpirit : public Character
{
public:
    FadingSpirit(int level)
        : Character("Fading Spirit", "Apparition", 's' | COLOR_PAIR(UI::ColorPairs::BlackOnDefault) | A_BOLD, 4)
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
        newStats.Sorcery   = lround(9 + (level - 1) * 1.9);
        newStats.Wisdom    = lround(8 + (level - 1) * 1.0);
        newStats.MaxMana   = newStats.Wisdom;
        newStats.Mana      = newStats.Wisdom;
        return newStats;
    }
};

/**
 * @brief Level 1-10 weak Animal
 */
class Rat : public Character
{
public:
    Rat(int level) : Character("Rat", "Animal", 'r', 3)
    {
        m_Stats = CalculateBaseStatsForLevel(level);
        GrantSkill<Battle::SkillCollection::Nibble>();
    }

    Stats CalculateBaseStatsForLevel(int level) const override
    {
        Stats newStats;
        newStats.Level     = level;
        newStats.MaxHealth = lround(10 + (level - 1) * 1.6);
        newStats.Health    = newStats.MaxHealth;
        newStats.Strength  = lround(6 + (level - 1 * 0.9));
        newStats.Dexterity = lround(21 + (level - 1) * 2.4);
        newStats.Sorcery   = 0;
        newStats.Wisdom    = 0;
        newStats.MaxMana   = newStats.Wisdom;
        newStats.Mana      = newStats.Wisdom;
        return newStats;
    }
};

} /* namespace Entities::NPCCollection */
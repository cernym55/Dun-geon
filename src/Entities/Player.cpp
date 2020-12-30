#include "Player.h"
#include "Battle/SkillCollection.h"
#include "Misc/Direction.h"
#include "Misc/Exceptions.h"
#include <cmath>
#include <ncurses.h>

namespace Entities
{

Player::Player(const std::string& name, chtype icon)
    : Character(name, "Duelist", icon, 0, CalculateBaseStatsForLevel(1)),
      m_XP(0),
      m_XPToLevelUp(CalculateXPToNextLevel(1)),
      m_Dun(0)
{
    GrantSkill<Battle::SkillCollection::Swing>();
}

Stats Player::CalculateBaseStatsForLevel(int level) const
{
    // For Duelist class
    Stats newStats;
    newStats.Level = level;

    // Polynomial scaling
    newStats.MaxHealth = lround(29.4422237959225 + (-3.70933306129882 * (level + 1))
                                + (0.77715937555331 * pow(level + 1, 2)) + (-0.0168688082142126 * pow(level + 1, 3))
                                + (0.000173568777515953 * pow(level + 1, 4)) + (-0.00000068184909 * pow(level + 1, 5)));
    newStats.Health    = newStats.MaxHealth;

    // Linear scaling         | Base + Per Level Gain
    // newStats.Toughness    = lround(15   + (level - 1) * 1.39795918367347);
    newStats.Strength  = lround(19 + (level - 1) * 1.60204081632653);
    newStats.Dexterity = lround(14 + (level - 1) * 1.07142857142857);
    newStats.Sorcery   = lround(7 + (level - 1) * 0.581632653061224);
    newStats.Wisdom    = lround(10 + (level - 1) * 0.948979591836735);

    newStats.MaxMana = newStats.Wisdom;
    newStats.Mana    = newStats.MaxMana;

    return newStats;
}

int Player::GetXP() const
{
    return m_XP;
}

int Player::GetXPToLevelUp() const
{
    return m_XPToLevelUp;
}

int Player::GetDun() const
{
    return m_Dun;
}

Direction Player::GetNextMove(const EntityManager& entityManager)
{
    throw NotSupportedException("Attempted to make unsupported call: Player::GetNextMove()");
}

void Player::SetHealth(int value)
{
    m_Stats.Health = value;
}

void Player::SetMana(int value)
{
    m_Stats.Mana = value;
}

bool Player::GrantXP(int howMuch)
{
    if (m_Stats.Level == Entities::LevelCap)
        return false;

    m_XP += howMuch;
    if (m_XP >= m_XPToLevelUp)
    {
        LevelUp();
        return true;
    }
    return false;
}

void Player::LevelUp()
{
    // Subtract XP points and calculate level gain
    int newLevel = m_Stats.Level;
    while (m_XP >= m_XPToLevelUp)
    {
        newLevel++;
        if (newLevel == Entities::LevelCap)
        {
            m_XP          = 0;
            m_XPToLevelUp = 0;
            break;
        }
        m_XP -= m_XPToLevelUp;
        m_XPToLevelUp = CalculateXPToNextLevel(newLevel);
    }

    // Change level
    m_Stats = CalculateBaseStatsForLevel(newLevel);
}

int Player::CalculateXPToNextLevel(int currentLevel) const
{
    static const int BaseRequirement = 15;
    return lround(BaseRequirement * sqrt(currentLevel) * pow(5.0 / 4.0, (currentLevel - 1) / 4.0));
}

} /* namespace Entities */

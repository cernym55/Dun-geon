#pragma once

#include "Character.h"
#include "Misc/Direction.h"
#include <ncurses.h>
#include <string>

namespace Entities
{

/**
 * @brief Player character class
 */
class Player : public Character
{
public:
    Direction FacingDirection;

    /**
     * @brief Constructor
     *
     * @param name name
     * @param icon icon (default: set to first character of name)
     */
    Player(const std::string& name, chtype icon = 0);

    /**
     * @brief Calculate the base stats at given level
     *
     * @param level player level
     * @return Stats base stats
     */
    Stats CalculateBaseStatsForLevel(int level) const override;

    /**
     * @brief Get XP
     */
    int GetXP() const;

    /**
     * @brief Get XP needed to level up
     */
    int GetXPToLevelUp() const;

    /**
     * @brief Get dun
     */
    int GetDun() const;

    /**
     * @brief GetNextMove is unsupported for Player
     *
     * @throw NotSupportedException
     */
    virtual Direction GetNextMove(const EntityManager& entityManager) override;

    /**
     * @brief Set player health
     *
     * @param value value
     */
    void SetHealth(int value);

    /**
     * @brief Set player mana
     *
     * @param value value
     */
    void SetMana(int value);

    /**
     * @brief Grant the player a set amount of XP
     *
     * @param howMuch how much
     * @return bool true if this causes the player to level up
     */
    bool GrantXP(int howMuch);

private:
    int m_XP;
    int m_XPToLevelUp;
    int m_Dun;

    /**
     * @brief Grants the player a level
     */
    void LevelUp();

    /**
     * @brief Calculate the amount of XP needed to advance to the next level
     *
     * @param currentLevel current level
     * @return int XP required
     */
    int CalculateXPToNextLevel(int currentLevel) const;
};

} /* namespace Entities */

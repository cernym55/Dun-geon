#pragma once

#include "Entities/Character.h"
#include "Entities/Player.h"

namespace UI { class BattleScreen; }

namespace Battle
{

class Battle
{
public:
    /**
     * @brief Result of the battle
     */
    enum class Result
    {
        Ongoing = -1,
        Victory = 0,
        GameOver = 1,
        Escape = 2
    };

    /**
     * @brief Constructor
     *
     * @param player player
     * @param enemy enemy character
     */
    Battle(Entities::Player& player, Entities::Character& enemy);

    /**
     * @brief Set the battle screen
     * 
     * @param battleScreen battle screen
     */
    void SetBattleScreen(UI::BattleScreen* battleScreen);

    /**
     * @brief Get the Player
     * 
     * @return const Entities::Player& player
     */
    const Entities::Player& GetPlayer() const;

    /**
     * @brief Get the Enemy
     * 
     * @return const Entities::Character& enemy
     */
    const Entities::Character& GetEnemy() const;

    /**
     * @brief Conduct the battle
     */
    Result DoBattle();

    const Entities::Character::Stats& GetPlayerStats() const;

private:
    Entities::Player& m_Player;
    Entities::Character& m_Enemy;
    UI::BattleScreen* m_BattleScreen;
    Result m_Result;
    Entities::Character::Stats m_PlayerStats;
    Entities::Character::Stats m_EnemyStats;

    /**
     * @brief Perform the player's turn
     */
    void DoPlayerTurn();

    /**
     * @brief Perform the enemy's turn
     */
    void DoEnemyTurn();

    /**
     * @brief Launch a player attack against the enemy
     * 
     * @param damage attack damage
     * @param hitChancePercent hit chance in percent
     */
    void LaunchPlayerAttack(int damage, int hitChancePercent);

    /**
     * @brief Wrap up
     */
    void FinishBattle();
};

} /* namespace Battle */
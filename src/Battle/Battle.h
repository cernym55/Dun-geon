#pragma once

#include "Skill.h"
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

    /**
     * @brief Get the current state of the player profile
     * 
     * @return const BattleProfile& player profile
     */
    const BattleProfile& GetPlayerProfile() const;

        /**
     * @brief Get the current state of the enemy profile
     * 
     * @return const BattleProfile& enemy profile
     */
    const BattleProfile& GetEnemyProfile() const;

private:
    Entities::Player& m_Player;
    Entities::Character& m_Enemy;
    UI::BattleScreen* m_BattleScreen;
    Result m_Result;
    BattleProfile m_PlayerProfile;
    BattleProfile m_EnemyProfile;

    /**
     * @brief Perform the player's turn
     */
    void DoPlayerTurn();

    /**
     * @brief Perform the enemy's turn
     */
    void DoEnemyTurn();

    /**
     * @brief Launch an attack
     * 
     * @param skill skill used
     * @param isPlayer true if the attacker is the player
     */
    void LaunchAttack(Skill& skill, bool isPlayer);

    /**
     * @brief Wrap up
     */
    void FinishBattle();

    /**
     * @brief Perform effect ticks and remove expired effects
     */
    void UpdateActiveEffects(BattleProfile& profile);
};

} /* namespace Battle */
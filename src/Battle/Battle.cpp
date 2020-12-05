#include "Battle.h"
#include "Misc/RNG.h"
#include "UI/BattleScreen.h"

namespace Battle
{

Battle::Battle(Entities::Player& player, Entities::Character& enemy)
    : m_Player(player),
      m_Enemy(enemy),
      m_BattleScreen(nullptr),
      m_Result(Result::Ongoing),
      m_PlayerStats(m_Player.GetStats()),
      m_EnemyStats(m_Enemy.GetStats())
{
}

void Battle::SetBattleScreen(UI::BattleScreen* battleScreen)
{
    m_BattleScreen = battleScreen;
}

const Entities::Player& Battle::GetPlayer() const
{
    return m_Player;
}

const Entities::Character& Battle::GetEnemy() const
{
    return m_Enemy;
}

Battle::Result Battle::DoBattle()
{
    bool playerTurn = true;
    while (m_Result == Result::Ongoing)
    {
        if (playerTurn)
        {
            DoPlayerTurn();
        }
        else
        {
            DoEnemyTurn();
        }
        playerTurn = !playerTurn;
    }

    FinishBattle();
    return m_Result;
}

const Entities::Character::Stats& Battle::GetPlayerStats() const
{
    return m_PlayerStats;
}

void Battle::DoPlayerTurn()
{
    if (m_PlayerStats.Health <= 0)
    {
        m_Result = Result::GameOver;
        return;
    }

    static std::map<int, std::string> actions
        = { { 0, "Melee" }, { 1, "Ranged" }, { 2, "Spell" }, { 3, "Brace" }, { 4, "Escape" } };
    actions.erase(1);
    actions.erase(2);
    // TODO: Filter actions based on what the player can actually do

ACTION_CHOICE:
    constexpr int RethinkCode = 1000;
    m_BattleScreen->PostMessage("What will " + m_Player.GetName() + " do?");

    int choice = m_BattleScreen->SelectPlayerAction(actions);
    switch (choice)
    {
    case 0:
    {
        m_BattleScreen->PostMessage("Which attack?");
        std::map<int, std::string> options = { { 0, "Swing" }, { RethinkCode, "<rethink>" } };
        std::map<int, std::pair<std::pair<int, int>, int>> attackStats = { { 0, { { 2, 5 }, 90 } } };

        int meleeChoice = m_BattleScreen->SelectWithHoverAction(options, [&](auto it) {
            m_BattleScreen->ClearProjectionArea();
            m_BattleScreen->ClearThumbnailArea();
            if (it->first == RethinkCode)
                return;
            const auto& stats = attackStats.at(it->first);
            m_BattleScreen->ProjectAttack(stats.second);
            m_BattleScreen->DrawSkillHoverThumbnail();
        });

        if (meleeChoice == RethinkCode)
            goto ACTION_CHOICE;
        m_BattleScreen->PostMessage("");

        m_BattleScreen->ClearProjectionArea();
        m_BattleScreen->ClearThumbnailArea();
        LaunchPlayerAttack(attackStats.at(meleeChoice).first, attackStats.at(meleeChoice).second);

        break;
    }
    case 4:
        m_Result = Result::Escape;
        break;
    default:
        m_Result = Result::Victory;
        break;
    }
}

void Battle::DoEnemyTurn()
{
    if (m_EnemyStats.Health <= 0)
    {
        m_Result = Result::Victory;
        return;
    }

    std::pair<std::pair<int, int>, int> attackStats = { { 2, 4 }, 60 };

    m_BattleScreen->PostMessage(m_Enemy.GetName() + " attacks!");

    LaunchEnemyAttack(attackStats.first, attackStats.second);
}

void Battle::LaunchPlayerAttack(std::pair<int, int> damageRange, int hitChancePercent)
{
    int damage = RNG::RandomInt(damageRange.first, damageRange.second) + m_PlayerStats.Strength / 10;
    bool hit = RNG::Chance(hitChancePercent / 100.);
    m_BattleScreen->AnimatePlayerAttack(damage, hit);
    if (hit)
    {
        m_EnemyStats.Health -= damage;
    }
}

void Battle::LaunchEnemyAttack(std::pair<int, int> damageRange, int hitChancePercent)
{
    int damage = RNG::RandomInt(damageRange.first, damageRange.second) + m_EnemyStats.Strength / 10;
    bool hit = RNG::Chance(hitChancePercent / 100.);
    m_BattleScreen->AnimateEnemyAttack(damage, hit);
    if (hit)
    {
        m_PlayerStats.Health -= damage;
    }
}

void Battle::FinishBattle()
{
    m_BattleScreen->BattleEndMessage(m_Result);

    // Update remaining player HP/MP
    m_Player.SetHealth(m_PlayerStats.Health);
    m_Player.SetMana(m_PlayerStats.Mana);
}

} /* namespace Battle */
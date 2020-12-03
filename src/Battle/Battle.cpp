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
        std::map<int, std::string> options             = { { 0, "Swing" }, { RethinkCode, "<rethink>" } };
        std::map<int, std::pair<int, int>> attackStats = { { 0, { m_Player.GetStats().Strength, 95 } } };

        int meleeChoice = m_BattleScreen->SelectWithHoverAction(options, [&](auto it) {
            m_BattleScreen->ClearProjectionArea();
            if (it->first == RethinkCode)
                return;
            const auto& stats = attackStats.at(it->first);
            m_BattleScreen->ProjectAttack(stats.first, stats.second);
        });
        if (meleeChoice == RethinkCode)
            goto ACTION_CHOICE;
        m_BattleScreen->PostMessage("");

        m_BattleScreen->ClearProjectionArea();
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

    // TODO
}

void Battle::LaunchPlayerAttack(int damage, int hitChancePercent)
{
    bool hit = RNG::Chance(hitChancePercent / 100.);
    m_BattleScreen->AnimatePlayerAttack(damage, hit);
    if (hit)
    {
        m_EnemyStats.Health -= damage;
    }
}

void Battle::FinishBattle()
{
    m_BattleScreen->BattleEndMessage(m_Result);
}

} /* namespace Battle */
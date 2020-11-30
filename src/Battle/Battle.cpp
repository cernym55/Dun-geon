#include "Battle.h"
#include "UI/BattleScreen.h"

namespace Battle
{

Battle::Battle(Entities::Player& player, Entities::Character& enemy)
    : m_Player(player),
      m_Enemy(enemy),
      m_BattleScreen(nullptr),
      m_Result(Result::Ongoing)
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
    while (m_Result == Result::Ongoing)
    {
        DoPlayerTurn();
        DoEnemyTurn();
    }

    // FinishBattle();
    return m_Result;
}

void Battle::DoPlayerTurn()
{
    std::map<int, std::string> actions
        = { { 0, "Melee" }, { 1, "Ranged" }, { 2, "Spell" }, { 3, "Brace" }, { 4, "Escape" } };
    // actions.erase(1);
    // actions.erase(2);
    // TODO: Filter actions based on what the player can actually do

ACTION_CHOICE:
    int choice = m_BattleScreen->SelectPlayerAction(actions);

    switch (choice)
    {
    case 0:
    {
        int damage = m_Player.GetStats().Vigor;

        // select melee attack
    }
    break;
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
    // TODO
}

} /* namespace Battle */
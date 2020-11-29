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

private:
    Entities::Player& m_Player;
    Entities::Character& m_Enemy;
    UI::BattleScreen* m_BattleScreen;
};

} /* namespace Battle */
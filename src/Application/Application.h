#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Player/Controller.h"
#include "UI/InputHandler.h"
#include "UI/Screen.h"
#include "Worlds/WorldManager.h"

namespace Application
{

/**
 * @brief Application encapsulation
 */
class Application
{
public:
    Application();

    void Run();

private:
    UI::InputHandler m_InputHandler;
    UI::Screen m_Screen;
    Worlds::WorldManager m_WorldManager;
    Entities::Player m_Player;
    Entities::EntityManager m_EntityManager;
    Player::Controller m_PlayerController;
};

} /* namespace Application */

#include "Application.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Direction.h"
#include "Player/Controller.h"
#include "UI/ColorPairs.h"
#include "UI/InputHandler.h"
#include "UI/Screen.h"
#include "Worlds/World.h"
#include "Worlds/WorldManager.h"
#include <ncurses.h>

namespace Application
{

Application::Application()
    : m_InputHandler(m_Screen, m_PlayerController),
    m_Screen(m_InputHandler, m_WorldManager, m_EntityManager, m_Player),
    m_WorldManager(),
    m_Player("Gref",
             { 1, 10, 10, 5, 10, 5, 5, 5, 10 },
             'G' | A_BOLD | COLOR_PAIR(UI::ColorPairs::MagentaOnDefault)),
    m_EntityManager(m_WorldManager, m_Player),
    m_PlayerController(m_EntityManager, m_WorldManager, m_Player)
{
}

void Application::Run()
{
    m_Screen.MainMenu();

    //TODO: move Player initialization out
    const Worlds::World& world = const_cast<const Worlds::WorldManager&>(m_WorldManager).CurrentWorld();
    m_Player.SetCoords({ static_cast<Coords::Scalar>(world.StartingRoom().GetWidth() / 2),
                         static_cast<Coords::Scalar>(world.StartingRoom().GetHeight() / 2) });
    m_EntityManager.TryMovePlayer(Direction::None);

    while (!m_InputHandler.ShouldQuit())
    {
        m_Screen.Draw();
        m_InputHandler.ProcessKeypress();
    }
}

} /* namespace Application */

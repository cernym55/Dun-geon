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
#include <sstream>
#include <ncurses.h>

namespace Application
{

Application::Application()
    : m_InputHandler(m_Screen, m_PlayerController),
    m_Screen(m_InputHandler, m_WorldManager, m_EntityManager, m_Player),
    m_WorldManager(),
    m_Player("Gref",
             'G' | A_BOLD | COLOR_PAIR(UI::ColorPairs::MagentaOnDefault)),
    m_EntityManager(m_WorldManager, m_Player),
    m_PlayerController(m_EntityManager, m_WorldManager, m_Player, m_Screen)
{
}

void Application::Run()
{
    try
    {
        m_Screen.MainMenu();

        while (!m_InputHandler.ShouldQuit())
        {
            m_Screen.Draw();
            m_InputHandler.ProcessKeypress();
        }
    }
    // Generic exception handler
    catch (std::exception& ex)
    {
        std::ostringstream message;
        message << ex.what()
                << "\n\nDun-geon will exit.";
        m_Screen.OkMessageBox(message.str(), "Error");
        return;
    }
}

} /* namespace Application */

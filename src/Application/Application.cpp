#include "Application.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Direction.h"
#include "Player/Controller.h"
#include "UI/Parser.h"
#include "UI/Screen.h"
#include "Worlds/World.h"
#include "Worlds/WorldManager.h"

namespace Application
{

Application::Application()
    : m_State(State::Initial),
      m_Parser(m_Screen, m_PlayerController),
      m_Screen(m_Parser, m_WorldManager, m_Player),
      m_WorldManager(),
      m_Player("Gref"),
      m_EntityManager(m_WorldManager, m_Player),
      m_PlayerController(m_EntityManager, m_WorldManager, m_Player)
{
}

void Application::Run()
{
    m_Screen.Clear();
    m_Screen.MainMenu();
    const Worlds::World& world = const_cast<const Worlds::WorldManager&>(m_WorldManager).GetCurrentWorld();
    //TODO: move Player initialization out
    m_Player.GetCoords() = { world.GetStartingRoom().GetWidth() / 2,
                             world.GetStartingRoom().GetHeight() / 2 };
    // m_Player.setBlocking(false);
    // m_Player.setName("Gref");
    // m_Player.setIcon(m_Player.getName()[0]);
    m_Player.setLastMove(Direction::None());
    m_Player.getStats().level = 1;
    m_Player.getStats().XP = 0;
    m_Player.getStats().XPToNextLevel = 0;
    m_Player.getStats().dun = 0;
    m_Player.getStats().health = 10;
    m_Player.getStats().healthMax = 10;
    m_Player.getStats().mana = 5;
    m_Player.getStats().manaMax = 10;
    m_Player.getStats().vigor = 5;
    m_Player.getStats().valor = 5;
    m_Player.getStats().haste = 5;
    m_Player.getStats().magic = 10;
    m_Screen.SetView(UI::Screen::View::World);
    while (!m_Parser.quit())
    {
        m_EntityManager.UpdateEntitiesInCurrentRoom();
        if (m_Parser.getCmdQueue().empty())
        {
            m_Screen.Draw();
            m_Parser.readInput();
            m_Parser.parse();
            m_Parser.eval();
        }
        m_Parser.execCommand();
    }
}

} /* namespace Application */
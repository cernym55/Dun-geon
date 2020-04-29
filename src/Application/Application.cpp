#include "Application.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Direction.h"
#include "Player/Controller.h"
#include "UI/InputHandler.h"
#include "UI/Screen.h"
#include "Worlds/World.h"
#include "Worlds/WorldManager.h"

namespace Application
{

Application::Application()
    : m_InputHandler(m_Screen, m_PlayerController),
      m_Screen(m_InputHandler, m_WorldManager, m_EntityManager, m_Player),
      m_WorldManager(),
      m_Player("Gref"),
      m_EntityManager(m_WorldManager, m_Player),
      m_PlayerController(m_EntityManager, m_WorldManager, m_Player)
{
}

void Application::Run()
{
    m_Screen.MainMenu();
    const Worlds::World& world = const_cast<const Worlds::WorldManager&>(m_WorldManager).GetCurrentWorld();
    //TODO: move Player initialization out
    m_Player.SetCoords({ world.GetStartingRoom().GetWidth() / 2,
                         world.GetStartingRoom().GetHeight() / 2 });
    m_Player.GetStats().level = 1;
    m_Player.GetStats().XP = 0;
    m_Player.GetStats().XPToNextLevel = 1;
    m_Player.GetStats().dun = 0;
    m_Player.GetStats().health = 10;
    m_Player.GetStats().healthMax = 10;
    m_Player.GetStats().mana = 5;
    m_Player.GetStats().manaMax = 10;
    m_Player.GetStats().vigor = 5;
    m_Player.GetStats().valor = 5;
    m_Player.GetStats().haste = 5;
    m_Player.GetStats().magic = 10;
    m_EntityManager.TryMovePlayerEntity(Direction::None());
    while (!m_InputHandler.quit())
    {
        // if (m_InputHandler.getCmdQueue().empty())
        // {
        //     m_Screen.Draw();
        //     m_InputHandler.readInput();
        //     m_InputHandler.parse();
        //     m_InputHandler.eval();
        // }
        // m_InputHandler.execCommand();
        m_Screen.Draw();
        m_InputHandler.HandleNextKeyInput();
    }
}

} /* namespace Application */
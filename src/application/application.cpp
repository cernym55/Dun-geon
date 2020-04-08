#include "application.h"
#include "ui/parser.h"
#include "ui/screen.h"
#include "worlds/world.h"
#include "worlds/world_manager.h"

namespace Application
{

Application::Application()
    : m_State(State::Initial), m_Parser(), m_Screen(m_Parser), m_Player(), m_WorldManager(*this)
{
}

void Application::Run()
{
    m_Screen.clear();
    m_Screen.mainMenu();
    //TODO: move this outside
    Worlds::World& world = m_WorldManager.GetCurrentWorld();
    m_Player.setPosX(world.getRoom(7, 7)->getDimX() / 2);
    m_Player.setPosY(world.getRoom(7, 7)->getDimY() / 2);
    m_Player.setBlocking(false);
    m_Player.setName("Gref");
    m_Player.setIcon(m_Player.getName()[0]);
    m_Player.setLastMove(Entities::up);
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
    m_Player.setCurrentRoom(world.getRoom(7, 7));
    m_Screen.setMode(UI::WORLD);
    m_Screen.setCurrentRoom(m_Player.getCurrentRoom());
    m_Screen.parser().setScreen(&m_Screen);
    while (!m_Screen.parser().quit())
    {
        // force screen to draw the room the player is in
        if (m_Screen.getCurrentRoom() != m_Player.getCurrentRoom())
        {
            m_Screen.setCurrentRoom(m_Player.getCurrentRoom());
        }
        m_Player.getCurrentRoom()->updateEntityPos();
        if (m_Screen.parser().getCmdQueue().empty())
        {
            m_Screen.draw();
            m_Screen.parser().readInput();
            m_Screen.parser().parse();
            m_Screen.parser().eval();
        }
        m_Screen.parser().execCommand();
    }
}

Entities::Player& Application::GetPlayer()
{
    return m_Player;
}

} /* namespace Application */
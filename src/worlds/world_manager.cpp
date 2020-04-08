#include "world_manager.h"
#include "application/application.h"
#include "room.h"
#include "world.h"
#include <memory>

namespace Worlds
{

WorldManager::WorldManager(Application::Application& application)
    : m_Application(application), m_NextWorldNum(1)
{
    // Generate a world
    //TODO: move this away
    m_Worlds.push_back(std::make_unique<World>(m_NextWorldNum, &m_Application.GetPlayer()));
    m_NextWorldNum++;
    m_Worlds.back()->getRoom(7, 7)->generate(Worlds::box, false, false, false, false);
}

World& WorldManager::GetCurrentWorld()
{
    return *m_Worlds.back();
}

} /* namespace Worlds */
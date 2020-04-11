#include "world_manager.h"
#include "entities/player.h"
#include "room.h"
#include "world.h"
#include <memory>

namespace Worlds
{

WorldManager::WorldManager(Entities::Player& player)
    : m_Player(player), m_NextWorldNum(1)
{
    // Generate a world
    //TODO: move this away
    m_Worlds.push_back(std::make_unique<World>(*this, m_NextWorldNum));
    m_NextWorldNum++;
    m_Worlds.back()->GetRoom(7, 7)->generate(Worlds::Layout::Box, false, false, false, false);
}

World& WorldManager::GetCurrentWorld()
{
    return *m_Worlds.back();
}

std::pair<int, int> WorldManager::GetPlayerPosition() const
{
    return std::pair<int, int>(m_Player.getPosX(), m_Player.getPosY());
}

const Entities::Player& WorldManager::GetPlayer() const
{
    return m_Player;
}

} /* namespace Worlds */
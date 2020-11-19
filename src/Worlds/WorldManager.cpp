#include "WorldManager.h"
#include "Misc/Coords.h"
#include "Misc/Direction.h"
#include "Room.h"
#include "World.h"
#include <memory>

namespace Worlds
{

WorldManager::WorldManager()
    : m_NextWorldNumber(1),
      m_CurrentWorld(nullptr),
      m_CurrentRoomCoords(World::CenterPos, World::CenterPos)
{
    World& firstWorld = CreateWorld();
    m_CurrentWorld = &firstWorld;
}

const World& WorldManager::CurrentWorld() const
{
    return *m_CurrentWorld;
}

Room& WorldManager::CurrentRoom()
{
    return m_CurrentWorld->RoomAt(m_CurrentRoomCoords);
}

const Room& WorldManager::CurrentRoom() const
{
    return m_CurrentWorld->RoomAt(m_CurrentRoomCoords);
}

bool WorldManager::IsCurrentRoom(const Room& room) const
{
    return room.GetCoords() == m_CurrentRoomCoords;
}

Room& WorldManager::SwitchRoom(Direction dir)
{
    Coords newCoords = m_CurrentRoomCoords.Adjacent(dir);
    if (!m_CurrentWorld->RoomExists(newCoords))
    {
        m_CurrentWorld->CreateRoom(newCoords);
    }
    m_CurrentRoomCoords = newCoords;
    return CurrentRoom();
}

World& WorldManager::CurrentWorld()
{
    return *m_CurrentWorld;
}

World& WorldManager::CreateWorld()
{
    m_Worlds.push_back(std::make_unique<World>(*this, PopWorldNumber()));

    return *m_Worlds.back();
}

int WorldManager::PopWorldNumber()
{
    return m_NextWorldNumber++;
}

} /* namespace Worlds */
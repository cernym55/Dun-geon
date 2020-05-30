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

const World& WorldManager::GetCurrentWorld() const
{
    return *m_CurrentWorld;
}

Room& WorldManager::GetCurrentRoom()
{
    return m_CurrentWorld->GetRoomAt(m_CurrentRoomCoords);
}

const Room& WorldManager::GetCurrentRoom() const
{
    return m_CurrentWorld->GetRoomAt(m_CurrentRoomCoords);
}

bool WorldManager::IsCurrentRoom(const Room& room) const
{
    return room.GetCoords() == m_CurrentRoomCoords;
}

Room& WorldManager::SwitchCurrentRoom(Direction dir)
{
    Coords newCoords = m_CurrentRoomCoords.Adjacent(dir);
    if (!m_CurrentWorld->RoomExistsAt(newCoords))
    {
        m_CurrentWorld->CreateRoomAt(newCoords);
    }
    m_CurrentRoomCoords = newCoords;
    return GetCurrentRoom();
}

World& WorldManager::GetCurrentWorld()
{
    return *m_CurrentWorld;
}

World& WorldManager::CreateWorld()
{
    m_Worlds.push_back(std::make_unique<World>(*this, PopNextWorldNumber()));

    return *m_Worlds.back();
}

int WorldManager::PopNextWorldNumber()
{
    return m_NextWorldNumber++;
}

} /* namespace Worlds */
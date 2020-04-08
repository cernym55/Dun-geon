#pragma once

#include "application/application_forward.h"
#include "world.h"
#include <memory>
#include <vector>

namespace Worlds
{

/**
 * @brief Class for managing Worlds and their creation
 */
class WorldManager
{
public:
    /**
     * @brief Constructor
     * 
     * @param application Application
     */
    WorldManager(Application::Application& application);

    /**
     * @brief Delete copy constructor
     */
    WorldManager(const WorldManager&) = delete;

    /**
     * @brief Default destructor
     */
    ~WorldManager() = default;

    /**
     * @brief Get the currently active world.
     * 
     * @return World& current world
     */
    World& GetCurrentWorld();

private:
    Application::Application& m_Application;
    std::vector<std::unique_ptr<World>> m_Worlds;
    int m_NextWorldNum;
};

} /* namespace Worlds */
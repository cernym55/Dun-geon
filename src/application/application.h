#pragma once

#include "entities/player.h"
#include "ui/parser.h"
#include "ui/screen.h"
#include "worlds/world_manager.h"

namespace Application
{

/**
 * @brief Global application class
 */
class Application
{
public:
    /**
     * @brief Constructor
     */
    Application();

    /**
     * @brief Delete copy constructor
     */
    Application(const Application&) = delete;

    /**
     * @brief Default destructor
     */
    ~Application() = default;

    /**
     * @brief Run the application
     */
    void Run();

private:
    /**
     * @brief Application state
     */
    enum class State
    {
        /**
         * @brief Initial state assigned to the application by default
         */
        Initial = 0,

        /**
         * @brief Main menu is active
         */
        InMainMenu,

        /**
         * @brief A game is running
         */
        InGame
    } m_State;

    UI::Parser m_Parser;
    UI::Screen m_Screen;
    Entities::Player m_Player;
    Worlds::WorldManager m_WorldManager;
};

} /* namespace Application */
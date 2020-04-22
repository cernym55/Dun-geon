#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Player/Controller.h"
#include "UI/Parser.h"
#include "UI/Screen.h"
#include "Worlds/WorldManager.h"

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

    Application(const Application&) = delete;

    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;

    Application& operator=(Application&&) = delete;

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
    Worlds::WorldManager m_WorldManager;
    Entities::Player m_Player;
    Entities::EntityManager m_EntityManager;
    Player::Controller m_PlayerController;
};

} /* namespace Application */
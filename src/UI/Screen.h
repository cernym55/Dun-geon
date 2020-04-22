#pragma once

#include "Application/Application_forward.h"
#include "Entities/Player.h"
#include "Entities/EntityManager.h"
#include "Misc/Coords.h"
#include "Parser.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include <iostream>

namespace UI
{

/**
 * @brief Manager for text display and UI
 */
class Screen
{
public:
    /**
     * @brief Primary interface view screens
     * 
     */
    enum class View
    {
        /**
         * @brief View of the game world
         */
        World,

        /**
         * @brief Test
         */
        Test,

        /**
         * @brief View of the player's inventory
         */
        Inventory
    };

    /**
     * @brief Constructor
     * 
     * @param parser parser
     * @param worldManager world manager
     * @param entityManager entity manager
     * @param player player entity
     */
    Screen(Parser& parser,
           const Worlds::WorldManager& worldManager,
           const Entities::EntityManager& entityManager,
           const Entities::Player& player);

    Screen(const Screen&) = delete;

    Screen(Screen&&) = delete;

    Screen& operator=(const Screen&) = delete;

    Screen& operator=(Screen&&) = delete;

    /**
     * @brief Destructor
     */
    ~Screen() = default;

    /**
     * @brief Get the Parser object
     * 
     * @return Parser& parser
     */
    Parser& GetParser();
    void MainMenu();
    void PrintCenter(std::string str, int spaceWidth, bool secondPad);
    void Draw();
    void Prompt();
    void Clear();
    View GetView();
    void SetView(View m);

private:
    Parser& m_Parser;
    const Worlds::WorldManager& m_WorldManager;
    const Entities::EntityManager& m_EntityManager;
    const Entities::Player& m_Player;
    View m_View;
    std::string GetMapRow(int rowNumber);
    void PrintHUDRow(int rowNumber);
    char GetFieldIcon(const Worlds::Field& field) const;
    char GetFieldIcon(Coords coords) const;
};

} /* namespace UI */
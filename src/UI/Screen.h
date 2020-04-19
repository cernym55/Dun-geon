#pragma once

#include "Application/Application_forward.h"
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
     */
    Screen(Parser& parser, const Worlds::WorldManager& worldManager, const Entities::Player& player);

    /**
     * @brief Delete copy constructor
     * 
     */
    Screen(const Screen&) = delete;

    /**
     * @brief Default destructor
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
    const Entities::Player& m_Player;
    View m_View;
    std::string GetMapRow(int rowNumber);
    void PrintHUDRow(int rowNumber);
    char GetFieldIcon(const Worlds::Field& field) const;
    char GetFieldIcon(Coords coords) const;
};

} /* namespace UI */
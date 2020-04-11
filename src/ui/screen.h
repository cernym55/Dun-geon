#pragma once

#include "application/application_forward.h"
#include "parser.h"
#include "worlds/room.h"
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
    Screen(Parser& parser, Worlds::WorldManager& worldManager);

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
    Worlds::Room* GetCurrentRoom();
    void SetCurrentRoom(Worlds::Room* roomPtr);

private:
    Parser& m_Parser;
    Worlds::WorldManager& m_WorldManager;
    Worlds::Room* m_CurrentRoom;
    View m_View;
    std::string GetMapRow(int rowNumber);
    void PrintHUDRow(int rowNumber);
    char GetFieldIcon(int fieldX, int fieldY);
};

} /* namespace UI */
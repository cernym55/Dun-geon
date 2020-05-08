#pragma once

#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "InputHandler.h"
#include "Misc/Coords.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include <iostream>
#include <map>
#include <ncurses.h>
#include <string>

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
         * @brief Main menu view
         */
        MainMenu,

        /**
         * @brief View of the game world
         */
        World,

        /**
         * @brief View of the player's inventory
         */
        Inventory
    };

    /**
     * @brief Width of the screen
     */
    constexpr static const Coords::Scalar ScreenWidth = 80;

    /**
     * @brief Height of the screen
     */
    constexpr static const Coords::Scalar ScreenHeight = 25;

    /**
     * @brief Width of the world panel
     */
    constexpr static const Coords::Scalar WorldPanelWidth = 50;

    /**
     * @brief Height of the world panel
     */
    constexpr static const Coords::Scalar WorldPanelHeight = 21;

    /**
     * @brief Width of the HUD panel
     */
    constexpr static const Coords::Scalar HUDPanelWidth = ScreenWidth - WorldPanelWidth;

    /**
     * @brief Constructor
     * 
     * @param inputHandler input handler
     * @param worldManager world manager
     * @param entityManager entity manager
     * @param player player entity
     */
    Screen(InputHandler& inputHandler,
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
    ~Screen();

    /**
     * @brief Display the main menu
     */
    void MainMenu();

    /**
     * @brief Draw the game screen
     */
    void Draw();

    /**
     * @brief Get the view
     * 
     * @return View view
     */
    View GetView() const;

    /**
     * @brief Set the message to display the next time the screen is drawn
     */
    void PostMessage(const std::string& message);

    /**
     * @brief Show a centered message box with two menu-like option buttons and a variable prompt
     * 
     * @param prompt prompt text (may contain newlines, message box will be adjusted)
     * @param leftOption left button option (default: "Yes")
     * @param rightOption right button option (default: "No")
     * @param title window title (default: blank)
     * @return true if left option was picked
     * @return false if right option was picked
     */
    bool YesNoMessageBox(const std::string& prompt, const std::string& leftOption = "Yes", const std::string& rightOption = "No", const std::string& title = "");

private:
    /**
     * @brief Default icon for empty fields
     */
    constexpr static const chtype DefaultFieldIcon = ' ';

    InputHandler& m_InputHandler;
    const Worlds::WorldManager& m_WorldManager;
    const Entities::EntityManager& m_EntityManager;
    const Entities::Player& m_Player;
    View m_View;
    WINDOW* m_GameWorldWindow;
    WINDOW* m_GameHUDWindow;
    WINDOW* m_GameMessageWindow;
    const Worlds::Room* m_CurrentRoom;
    std::string m_Message;

    /**
     * @brief Initialize the screen
     */
    void Init();

    /**
     * @brief Terminate the screen
     */
    void Terminate();

    /**
     * @brief Print the string centered on line yPos
     * 
     * @param str string
     * @param yPos Y position
     */
    void PrintCenterAt(const std::string& str, int yPos);

    /**
     * @brief Print the string centered in given window on line yPos
     * 
     * @param window window
     * @param str string
     * @param yPos Y position
     */
    void PrintCenterAt(WINDOW* window, const std::string& str, int yPos);

    /**
     * @brief Draw the Dun-geon logo at the selected position
     * 
     * @param xPos X
     * @param yPos Y
     */
    void DrawLogo(int xPos = 11, int yPos = 3);

    /**
     * @brief Draw a menu prompt and return the id associated with the selected option
     * 
     * @param options map of id:label option pairs
     * @param position position of the upper left corner of the menu
     * @param width width of the menu (will trim option labels if too narrow)
     * @param height height of the menu (will scroll if too short for all options)
     * @param drawBorder whether or not to draw a border around the menu (default: true)
     * @param padX padding between the side borders and menu options (default: 1)
     * @param padY padding between the top and bottom borders and menu options (default: 1)
     * @param title menu box title (default: blank)
     * @param spaceOptions whether or not to insert a blank line between each two options (default: false)
     * @return int id of the selected option
     */
    int SelectViaMenu(std::map<int, std::string> options, Coords position, int width, int height, bool drawBorder = true, int padX = 0, int padY = 0, const std::string& title = "", bool spaceOptions = false);

    /**
     * @brief Initialize to display the game screen
     */
    void StartGame();

    /**
     * @brief Resize and reposition the world window according to current room dimensions
     */
    void ResizeAndRepositionWorldWindow();

    /**
     * @brief Draw the world window
     */
    void DrawWorld();

    /**
     * @brief Draw the HUD window
     */
    void DrawHUD();

    /**
     * @brief Draw the message window
     */
    void DrawMessageWindow();

    /**
     * @brief Get the icon for the given field
     * 
     * @param field field
     * @return chtype icon
     */
    chtype GetFieldIcon(const Worlds::Field& field) const;

    /**
     * @brief Get the icon for the field at the given coords
     * 
     * @param coords coords
     * @return chtype icon
     */
    chtype GetFieldIcon(Coords coords) const;
};

} /* namespace UI */
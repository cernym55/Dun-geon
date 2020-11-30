#pragma once

#include "Battle/Battle.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "InputHandler.h"
#include "Misc/Coords.h"
#include "Subscreen.h"
#include "WorldMapObjectType.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include <functional>
#include <iostream>
#include <map>
#include <ncurses.h>
#include <string>

namespace UI
{

class BattleScreen;

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
         * @brief In game view
         */
        InGame,

        /**
         * @brief World map view
         */
        Map,

        /**
         * @brief Battle view
         */
        Battle
    };

    /**
     * @brief Width of the screen
     */
    constexpr static const int ScreenWidth = 80;

    /**
     * @brief Height of the screen
     */
    constexpr static const int ScreenHeight = 25;

    /**
     * @brief Width of the world panel
     */
    constexpr static const int WorldPanelWidth = 50;

    /**
     * @brief Height of the world panel
     */
    constexpr static const int WorldPanelHeight = 21;

    /**
     * @brief Width of the HUD panel
     */
    constexpr static const int HUDPanelWidth = ScreenWidth - WorldPanelWidth;

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
     * @brief Clear the screen
     */
    void Clear();

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
     * @brief Show the world map in a window
     */
    void ShowMap();

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

    /**
     * @brief Show a centered message box with an OK button
     *
     * @param message message
     * @param title window title (default: blank)
     * @param buttonLabel text on the button
     */
    void OkMessageBox(const std::string& message, const std::string& title = "", const std::string& buttonLabel = "OK");

    /**
     * @brief Create a battle screen
     * 
     * @param battle battle
     * @return BattleScreen& battle screen
     */
    BattleScreen& OpenBattleScreen(Battle::Battle& battle);

    /**
     * @brief Close the subscreen
     */
    void CloseSubscreen();

    /**
     * @brief Print the string centered on line yPos
     * 
     * @param str string
     * @param yPos Y position
     */
    static void PrintCenter(const std::string& str, int yPos);

    /**
     * @brief Print the string centered in given window on line yPos
     * 
     * @param window window
     * @param str string
     * @param yPos Y position
     */
    static void PrintCenter(WINDOW* window, const std::string& str, int yPos);

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
     * @param scroll whether the menu should scroll or have multiple columns if options won't fit (default: scroll)
     * @param hoverAction action called with the highlighted item whenever moving the menu cursor (default: none)
     * @return int id of the selected option
     */
    static int SelectViaMenu(std::map<int, std::string> options,
                             Coords position,
                             int width,
                             int height,
                             bool drawBorder                                                       = true,
                             int padX                                                              = 0,
                             int padY                                                              = 0,
                             const std::string& title                                              = "",
                             bool spaceOptions                                                     = false,
                             bool scroll                                                           = true,
                             std::function<void(std::map<int, std::string>::iterator)> hoverAction = {});

private:
    /**
     * @brief Default icon for empty fields
     */
    constexpr static const chtype DefaultFieldIcon = ' ';

    constexpr static const int WorldMapWidth = Worlds::World::MaximumSpan * 2 - 1 + 2;
    constexpr static const int WorldMapHeight = Worlds::World::MaximumSpan + 2;
    constexpr static const int WorldMapXPos = (ScreenWidth - WorldMapWidth) / 2;
    constexpr static const int WorldMapYPos = (ScreenHeight - WorldMapHeight) / 2;

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
    bool m_IsWorldMapCursorEnabled;
    std::unique_ptr<Subscreen> m_Subscreen;

    /**
     * @brief Initialize the screen
     */
    void Init();

    /**
     * @brief Terminate the screen
     */
    void Terminate();

    /**
     * @brief Draw the Dun-geon logo at the selected position
     * 
     * @param xPos X
     * @param yPos Y
     */
    void DrawLogo(int xPos = 11, int yPos = 3);

    /**
     * @brief Initialize to display the game screen
     */
    void StartGame();

    /**
     * @brief Resize and reposition the world window according to current room dimensions
     */
    void ResizeWorldWindow();

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
     * 
     * @param shouldPostMessage whether or not to write out the current message
     */
    void DrawMessageWindow(bool shouldPostMessage = true);

    /**
     * @brief Draw the map in the map window
     * If cursor coords are set to a non-default value, will draw the cursor
     * at the specified position.
     * 
     * @param mapWindow map window
     * @param cursor cursor position on world grid
     */
    void DrawMap(WINDOW* mapWindow, Coords cursor = { -1, -1 });

    /**
     * @brief Draw the tooltip for the object under the cursor
     * 
     * @param cursor cursor
     * @param objectType object type
     */
    void DrawMapTooltip(Coords cursor, WorldMapObjectType objectType);

    /**
     * @brief Get the icon for the given field
     * 
     * @param field field
     * @return chtype icon
     */
    chtype FieldIcon(const Worlds::Field& field) const;

    /**
     * @brief Get the icon for the field at the given coords
     * 
     * @param coords coords
     * @return chtype icon
     */
    chtype FieldIcon(Coords coords) const;

    /**
     * @brief Get the map icon for this room
     * 
     * @param room room
     * @return chtype map icon
     */
    chtype RoomMapIcon(const Worlds::Room& room) const;

    /**
     * @brief Get the WorldMapObjectType for the given coords
     * 
     * @param coords coords
     * @return WorldMapObjectType object type
     */
    WorldMapObjectType MapObjectType(Coords coords) const;
};

} /* namespace UI */

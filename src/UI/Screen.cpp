#include "Screen.h"
#include "BattleScreen.h"
#include "CameraStyle.h"
#include "ColorPairs.h"
#include "Components/FillBar.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "InputHandler.h"
#include "Misc/Coords.h"
#include "Misc/Exceptions.h"
#include "Misc/RNG.h"
#include "Misc/Utils.h"
#include "WorldMapObjectType.h"
#include "Worlds/Field.h"
#include "Worlds/Generation/RoomLayout.h"
#include "Worlds/Room.h"
#include "Worlds/World.h"
#include "Worlds/WorldManager.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
#include <menu.h>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <thread>

#define PLAYER_HEALTH_PC std::lround(m_Player.GetStats().Health / 1.0 / m_Player.GetStats().MaxHealth * 100)
#define PLAYER_MANA_PC   std::lround(m_Player.GetStats().Mana / 1.0 / m_Player.GetStats().MaxMana * 100)

namespace UI
{

Screen::Screen(InputHandler& inputHandler,
               const Worlds::WorldManager& worldManager,
               const Entities::EntityManager& entityManager,
               const Entities::Player& player)
    : m_InputHandler(inputHandler),
      m_WorldManager(worldManager),
      m_EntityManager(entityManager),
      m_Player(player),
      m_View(View::MainMenu),
      m_GameWorldWindow(nullptr),
      m_GameHUDWindow(nullptr),
      m_GameMessageWindow(nullptr),
      m_CurrentRoom(nullptr),
      m_IsWorldMapCursorEnabled(true)
{
    Init();
}

Screen::~Screen()
{
    Terminate();
}

void Screen::MainMenu()
{
    m_View                                          = View::MainMenu;
    static const std::vector<std::string> splashMsg = { "Speakest not of it.",
                                                        "And you thought you'd seen it all...",
                                                        "Enough dun for everyone.",
                                                        "Not nearly as much mashing required this time.",
                                                        "Have you brought your sporks?",
                                                        "The gate opens, and dun awaits.",
                                                        "Not to be confused with a Dun-gen.",
                                                        "...Maybe this time they'll explain what dun is?",
                                                        "Your dun is no longer your own.",
                                                        "I dun my robe and wizard hat.",
                                                        "The dun shines brighter than ever before.",
                                                        "1500 dun well spent.",
                                                        "Hopefully it will have been worth the wait." };
    DrawLogo();
    int splashNumber = RNG::RandomInt(splashMsg.size());
    PrintCenter(splashMsg[splashNumber], 13);

    refresh();

    std::map<int, std::string> options = { { 0, "Start Game" }, { 2, "Quit" } };
    static const int menuWidth         = 20;
    static const int menuHeight        = 9;
    int choice
        = SelectViaMenu(options, (ScreenWidth - menuWidth) / 2 - 1, 15, menuWidth, menuHeight, false, 2, 1, "", true);
    switch (choice)
    {
    case 0:
        erase();
        refresh();
        StartGame();
        break;
    case 2:
        m_InputHandler.SetQuit();
        break;
    }
}

void Screen::Draw()
{
    DrawWorld();
    DrawHUD();
    DrawMessageWindow();
}

void Screen::Clear()
{
    werase(m_GameWorldWindow);
    wrefresh(m_GameWorldWindow);
    werase(m_GameHUDWindow);
    wrefresh(m_GameHUDWindow);
    wclear(m_GameMessageWindow);
    wrefresh(m_GameMessageWindow);
}

Screen::View Screen::GetView() const
{
    return m_View;
}

void Screen::PostMessage(const std::string& message)
{
    m_Message = message;
}

void Screen::ShowMap()
{
    View previousView = m_View;
    m_View            = View::Map;

    // Prepare window
    // Every room icon is 1 char wide and has 1 char of spacing on both sides horizontally
    // for drawing "hallways" between rooms. This also helps keep better proportions.
    WINDOW* mapWindow = newwin(WorldMapHeight, WorldMapWidth, WorldMapYPos, WorldMapXPos);

    // Handle map interaction
    Coords cursor = m_CurrentRoom->GetCoords();
    std::optional<chtype> key;
    bool done        = false;
    bool actionTaken = true;
    keypad(mapWindow, 1);

    do
    {
        // Need to redraw the screen to get rid of leftover tooltips
        if (actionTaken)
        {
            DrawWorld();
            DrawHUD();
            DrawMessageWindow(false); // don't post the message

            DrawMap(mapWindow, cursor);
            if (m_IsWorldMapCursorEnabled)
            {
                WorldMapObjectType selectedObjectType = MapObjectType(cursor);
                DrawMapTooltip(cursor, selectedObjectType);
            }

            actionTaken = false;
        }

        key = InputHandler::ReadKeypress(
            { 'w', KEY_UP, 'd', KEY_RIGHT, 's', KEY_DOWN, 'a', KEY_LEFT, ' ', KEY_ENTER, 10, 27, 'q' }, mapWindow);
        if (!key)
            continue;

        switch (key.value())
        {
        case 'w':
        case KEY_UP:
            if (m_IsWorldMapCursorEnabled && !m_WorldManager.CurrentWorld().IsAtWorldGridEdge(cursor, Direction::Up))
            {
                cursor.Move(Direction::Up);
                actionTaken = true;
            }
            break;
        case 'd':
        case KEY_RIGHT:
            if (m_IsWorldMapCursorEnabled && !m_WorldManager.CurrentWorld().IsAtWorldGridEdge(cursor, Direction::Right))
            {
                cursor.Move(Direction::Right);
                actionTaken = true;
            }
            break;
        case 's':
        case KEY_DOWN:
            if (m_IsWorldMapCursorEnabled && !m_WorldManager.CurrentWorld().IsAtWorldGridEdge(cursor, Direction::Down))
            {
                cursor.Move(Direction::Down);
                actionTaken = true;
            }
            break;
        case 'a':
        case KEY_LEFT:
            if (m_IsWorldMapCursorEnabled && !m_WorldManager.CurrentWorld().IsAtWorldGridEdge(cursor, Direction::Left))
            {
                cursor.Move(Direction::Left);
                actionTaken = true;
            }
            break;
        case ' ':
            m_IsWorldMapCursorEnabled = !m_IsWorldMapCursorEnabled;
            actionTaken               = true;
            break;
        case KEY_ENTER:
        case 10:
        case 27:
        case 'q':
            done = true;
            break;
        }
    } while (!done);

    // Clean up the window
    wclear(mapWindow);
    wrefresh(mapWindow);

    delwin(mapWindow);
    m_View = previousView;
}

bool Screen::YesNoMessageBox(const std::string& prompt,
                             const std::string& leftOption,
                             const std::string& rightOption,
                             const std::string& title)
{
    // Split the prompt into lines
    std::vector<std::string> lines;
    size_t neededWidth = 0;
    std::string lineBuf;
    std::istringstream iss(prompt);
    while (std::getline(iss, lineBuf))
    {
        if (lineBuf.size() > neededWidth)
            neededWidth = lineBuf.size();
        lines.push_back(std::move(lineBuf));
    }

    std::string left  = "  " + leftOption + "  ";
    std::string right = "  " + rightOption + "  ";
    // The ncurses menu forces all option boxes to be the same size.
    // Thus we should give the shorter option some padding
    std::string& longer  = left.size() > right.size() ? left : right;
    std::string& shorter = left.size() > right.size() ? right : left;
    for (size_t i = 0; i < (longer.size() - shorter.size()) / 2; i++)
    {
        shorter.insert(shorter.begin(), ' ');
    }

    int height = lines.size() + 4;
    int width  = neededWidth + 4;

    std::vector<ITEM*> items
        = { new_item(left.c_str(), left.c_str()), new_item(right.c_str(), right.c_str()), nullptr };

    int subXPos = (width - (left.size() + right.size() + 4)) / 2;

    WINDOW* boxWin = newwin(height, width, (ScreenHeight - height) / 2, (ScreenWidth - width) / 2);
    WINDOW* boxSub = derwin(boxWin, 1, width - subXPos - 3, height - 2, subXPos);
    MENU* menu     = new_menu(items.data());

    set_menu_win(menu, boxWin);
    set_menu_sub(menu, boxSub);
    keypad(boxWin, 1);
    menu_opts_off(menu, O_SHOWDESC);
    set_menu_mark(menu, "");
    set_menu_format(menu, 1, 2);
    set_menu_spacing(menu, 1, 1, 4);

    box(boxWin, 0, 0);
    wattron(boxWin, A_REVERSE);
    if (!title.empty())
        PrintCenter(boxWin, " " + title + " ", 0);
    wattroff(boxWin, A_REVERSE);
    for (size_t i = 0; i < lines.size(); i++)
    {
        PrintCenter(boxWin, lines[i], i + 1);
    }

    post_menu(menu);
    wrefresh(boxWin);
    bool selectedLeft = true;

    bool pressedEnter = false;
    std::optional<chtype> key;
    while (!pressedEnter)
    {
        key = InputHandler::ReadKeypress({ KEY_RIGHT, 'd', KEY_LEFT, 'a', 27, KEY_ENTER, 10 }, boxWin);
        if (!key)
            continue;

        switch (key.value())
        {
        // In a 2-button message box, the behavior depends on the selected option and not the key pressed
        case KEY_RIGHT:
        case 'd':
        case KEY_LEFT:
        case 'a':
            if (selectedLeft)
                menu_driver(menu, REQ_RIGHT_ITEM);
            else
                menu_driver(menu, REQ_LEFT_ITEM);
            selectedLeft = !selectedLeft;
            break;
        // ESC exits with negative response
        case 27:
            selectedLeft = false;
            pressedEnter = true;
            break;
        case KEY_ENTER:
        case 10:
            pressedEnter = true;
            break;
        }
        wrefresh(boxWin);
    }

    unpost_menu(menu);
    free_menu(menu);
    wclear(boxWin);
    wrefresh(boxWin);
    delwin(boxSub);
    delwin(boxWin);
    for (auto& item : items)
    {
        free_item(item);
    }

    return selectedLeft;
}

void Screen::OkMessageBox(const std::string& message, const std::string& title, const std::string& buttonLabel)
{
    // Split the message into lines
    std::vector<std::string> lines;
    size_t neededWidth = 0;
    std::string lineBuf;
    std::istringstream iss(message);
    while (std::getline(iss, lineBuf))
    {
        if (lineBuf.size() > neededWidth)
            neededWidth = lineBuf.size();
        lines.push_back(std::move(lineBuf));
    }

    int height         = lines.size() + 5;
    int width          = neededWidth + 4;
    std::string button = "  " + buttonLabel + "  ";

    WINDOW* boxWin = newwin(height, width, (ScreenHeight - height) / 2, (ScreenWidth - width) / 2);

    // Draw the window and contents
    box(boxWin, 0, 0);
    wattron(boxWin, A_REVERSE);
    if (!title.empty())
        PrintCenter(boxWin, " " + title + " ", 0);
    wattroff(boxWin, A_REVERSE);
    for (size_t i = 0; i < lines.size(); i++)
    {
        PrintCenter(boxWin, lines[i], i + 2);
    }

    // Shhhh... the button is not a real button, but no one has to be any the wiser
    wattron(boxWin, A_REVERSE);
    PrintCenter(boxWin, button, height - 2);
    wattroff(boxWin, A_REVERSE);

    wrefresh(boxWin);

    keypad(boxWin, 1);

    std::optional<chtype> key;
    while (!key.has_value() || !(key.value() == KEY_ENTER || key.value() == 10 || key.value() == 27))
    {
        key = InputHandler::ReadKeypress({ KEY_ENTER, 10, 27 }, boxWin);
    }

    wclear(boxWin);
    wrefresh(boxWin);

    delwin(boxWin);
}

BattleScreen& Screen::OpenBattleScreen(Battle::Battle& battle)
{
    BattleScreen* battleScreen = new BattleScreen(battle, *this, m_InputHandler);
    m_Subscreen.reset(battleScreen);
    battle.SetBattleScreen(battleScreen);
    // m_View = View::Battle;
    return *battleScreen;
}

void Screen::CloseSubscreen()
{
    if (m_Subscreen)
    {
        m_Subscreen.reset(nullptr);
    }
}

void Screen::GameOver(const std::string& epitaph)
{
    std::ostringstream additionalDeathInfo;
    additionalDeathInfo << "At the time of death, " << m_Player.GetName() << "\nwas carrying a sum of " << m_Player.GetDun()
                        << " dun.";

    OkMessageBox(epitaph + "\n\n" + additionalDeathInfo.str(), "GAME OVER", "R.I.P.");

    m_InputHandler.SetQuit();
}

void Screen::DisplayLevelUp(const Entities::Stats& prev, const Entities::Stats& next)
{
    // Assemble diff
    struct DiffEntry
    {
        std::string statName;
        int prev;
        int next;
        short color;
    };
    std::vector<DiffEntry> diff;

    if (next.MaxHealth > prev.MaxHealth)
    {
        diff.push_back({"Max Health", prev.MaxHealth, next.MaxHealth, ColorPairs::YellowOnDefault});
    }
    if (next.Strength > prev.Strength)
    {
        diff.push_back({"Strength", prev.Strength, next.Strength, ColorPairs::RedOnDefault});
    }
    if (next.Dexterity > prev.Dexterity)
    {
        diff.push_back({"Dexterity", prev.Dexterity, next.Dexterity, ColorPairs::GreenOnDefault});
    }
    if (next.Sorcery > prev.Sorcery)
    {
        diff.push_back({"Sorcery", prev.Sorcery, next.Sorcery, ColorPairs::MagentaOnDefault});
    }
    if (next.Wisdom > prev.Wisdom)
    {
        diff.push_back({"Wisdom", prev.Wisdom, next.Wisdom, ColorPairs::BlueOnDefault});
    }

    // Display window
    Draw();
    constexpr int width = 44;
    const int height    = 7 + diff.size();

    WINDOW* window = newwin(height, width, (ScreenHeight - height) / 2, (ScreenWidth - width) / 2);
    box(window, 0, 0);
    wattron(window, A_REVERSE);
    PrintCenter(window, " Level Up ", 0);
    PrintCenter(window, "  Continue  ", height - 2);
    wattroff(window, A_REVERSE);

    PrintCenter(window, m_Player.GetName() + " has advanced to level " + std::to_string(next.Level) + "!", 2);

    // First screen
    int diffLine = 0;
    for (const auto& diffEntry : diff)
    {
        wattron(window, A_BOLD | COLOR_PAIR(diffEntry.color));
        mvwprintw(window, 4 + diffLine, 9, "%s:", diffEntry.statName.c_str());
        mvwprintw(window, 4 + diffLine, 22, "%4d", diffEntry.prev);
        wattroff(window, A_BOLD | A_COLOR);
        waddstr(window, " -> ");
        wattron(window, COLOR_PAIR(diffEntry.color));
        wprintw(window, "%d", diffEntry.next);
        wattroff(window, A_BOLD | A_COLOR);
        diffLine++;
    }

    wrefresh(window);

    keypad(window, 1);

    std::optional<chtype> key;
    while (!key.has_value() || !(key.value() == KEY_ENTER || key.value() == 10 || key.value() == 27))
    {
        key = InputHandler::ReadKeypress({ KEY_ENTER, 10, 27 }, window);
    }

    PrintCenter(window, "            ", height - 2);

    // Animation
    for (int offset = 0; offset < 6; offset++)
    {
        diffLine = 0;
        for (const auto& diffEntry : diff)
        {
            mvwhline(window, 4 + diffLine, 22 + offset, ' ', 4);
            wattron(window, A_BOLD | COLOR_PAIR(diffEntry.color));
            
            if (offset < 5)
            {
                mvwprintw(window, 4 + diffLine, 23 + offset, "%4d", diffEntry.prev);
            }
            else
            {
                mvwprintw(window, 4 + diffLine, 23 + offset + 2, "%d", diffEntry.next);
            }

            wrefresh(window);
            
            wattroff(window, A_BOLD | A_COLOR);
            diffLine++;
        }
        if (offset < 5)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Second screen
    diffLine = 0;
    for (const auto& diffEntry : diff)
    {
        wattron(window, COLOR_PAIR(diffEntry.color));
        mvwprintw(window, 4 + diffLine, 24, "+%d", diffEntry.next - diffEntry.prev);
        wattroff(window, A_COLOR);
        wattroff(window, A_BOLD | A_COLOR);
        diffLine++;
    }

    wrefresh(window);

    wattron(window, A_REVERSE);
    PrintCenter(window, "  Close  ", height - 2);
    wattroff(window, A_REVERSE);

    key.reset();
    while (!key.has_value() || !(key.value() == KEY_ENTER || key.value() == 10 || key.value() == 27))
    {
        key = InputHandler::ReadKeypress({ KEY_ENTER, 10, 27 }, window);
    }

    werase(window);
    wrefresh(window);
    delwin(window);
}

void Screen::Init()
{
    initscr();
    start_color();
    use_default_colors();
    raw();
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    ESCDELAY = 0;

    ColorPairs::InitPairs();
}

void Screen::Terminate()
{
    if (m_GameWorldWindow != nullptr)
        delwin(m_GameWorldWindow);
    if (m_GameHUDWindow != nullptr)
        delwin(m_GameHUDWindow);
    if (m_GameMessageWindow != nullptr)
        delwin(m_GameMessageWindow);
    endwin();
}

void Screen::PrintCenter(const std::string& str, int yPos)
{
    int xPos = (ScreenWidth - str.size()) / 2;
    xPos += xPos % 2;
    mvaddstr(yPos, xPos, str.c_str());
    refresh();
}

void Screen::PrintCenter(WINDOW* window, const std::string& str, int yPos)
{
    int xPos = (getmaxx(window) - str.size()) / 2;
    mvwaddstr(window, yPos, xPos, str.c_str());
    wrefresh(window);
}

void Screen::DrawLogo(int xPos, int yPos)
{
    attron(A_BOLD);
    mvaddstr(++yPos, xPos, "_____");
    mvaddstr(++yPos, xPos, "|    \\   _   _   _  __         ____   ____   ____   _  __");
    mvaddstr(++yPos, xPos, "| /\\  \\ / \\ / \\ | |/  \\  ___  /    \\ / __ \\ /    \\ | |/  \\");
    mvaddstr(++yPos, xPos, "| \\/  / | |_| | |  _  | |___| | () | | ___/ | () | |  _  |");
    mvaddstr(++yPos, xPos, "\\____/  \\_____| \\_/ \\_|       \\__  | \\____/ \\____/ \\_/ \\_|");
    mvaddstr(++yPos, xPos + 31, "__| |");
    mvaddstr(++yPos, xPos + 30, "|____/  ");
    // Print the flashy version string
    addch('v');
    printw("%d", GameVersionMajor);
    addch('.');
    attron(COLOR_PAIR(ColorPairs::YellowOnDefault));
    printw("%d", GameVersionMinor);
    attroff(A_COLOR);
    addch('.');
    printw("%d", GameVersionRevision);
    attroff(A_BOLD);
}

int Screen::SelectViaMenu(std::map<int, std::string> options,
                          int xPos,
                          int yPos,
                          int width,
                          int height,
                          bool drawBorder,
                          int padX,
                          int padY,
                          const std::string& title,
                          bool spaceOptions,
                          bool scroll,
                          std::function<void(std::map<int, std::string>::iterator)> hoverAction)
{
    if (options.empty())
    {
        std::ostringstream errorMessage;
        errorMessage << "Attempted display of empty " << width << 'x' << height << " menu at " << Coords(xPos, yPos);
        if (!title.empty())
            errorMessage << " (\"" << title << "\")";
        throw DisplayException(errorMessage.str());
    }

    const size_t subWidth    = width - 2 - 2 * padX;
    const size_t subHeight   = height - 2 - 2 * padY;
    const size_t numRows     = spaceOptions ? (subHeight + subHeight % 2) / 2 : subHeight;
    const size_t numColumns  = scroll ? 1 : ceil(static_cast<double>(options.size()) / numRows);
    const size_t columnWidth = subWidth / numColumns;

    std::vector<ITEM*> items;
    for (auto& pair : options)
    {
        if (pair.second.size() > columnWidth)
        {
            ShortenString(pair.second, columnWidth);
        }
        size_t totalPad = columnWidth - pair.second.size();
        pair.second     = std::string(totalPad / 2, ' ') + pair.second + std::string(totalPad / 2 + totalPad % 2, ' ');
        items.push_back(new_item(pair.second.c_str(), pair.second.c_str()));
    }
    items.push_back(nullptr);
    MENU* menu         = new_menu(items.data());
    WINDOW* menuWindow = newwin(height, width, yPos, xPos);
    WINDOW* menuSub    = derwin(menuWindow, subHeight, subWidth, 1 + padY, 1 + padX);
    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, menuSub);
    keypad(menuWindow, 1);

    menu_opts_off(menu, O_SHOWDESC);
    set_menu_mark(menu, "");
    set_menu_spacing(menu, 1, spaceOptions ? 2 : 1, 1);
    set_menu_format(menu, numRows, numColumns);
    if (drawBorder)
        box(menuWindow, 0, 0);
    if (!title.empty())
        PrintCenter(menuWindow, title, 0);

    post_menu(menu);
    wrefresh(menuWindow);

    auto it       = options.begin();
    bool selected = false;
    std::optional<chtype> key;
    if (hoverAction)
        hoverAction(it);
    while (!selected)
    {
        key = InputHandler::ReadKeypress({ KEY_DOWN, 's', KEY_UP, 'w', KEY_RIGHT, 'd', KEY_LEFT, 'a', KEY_ENTER, 10 },
                                         menuWindow);
        if (!key)
            continue;

        // Position ascends horizontally
        size_t currentPosition = std::distance(options.begin(), it);

        switch (key.value())
        {
        case KEY_DOWN:
        case 's':
            if (currentPosition + numColumns < options.size())
            {
                menu_driver(menu, REQ_DOWN_ITEM);
                for (size_t i = 0; i < numColumns; i++)
                    it++;
                if (hoverAction)
                    hoverAction(it);
            }
            break;
        case KEY_UP:
        case 'w':
            if (currentPosition / numColumns > 0)
            {
                menu_driver(menu, REQ_UP_ITEM);
                for (size_t i = 0; i < numColumns; i++)
                    it--;
                if (hoverAction)
                    hoverAction(it);
            }
            break;
        case KEY_RIGHT:
        case 'd':
            if (!scroll && currentPosition % numColumns != numColumns - 1 && currentPosition + 1 < options.size())
            {
                menu_driver(menu, REQ_RIGHT_ITEM);
                it++;
                if (hoverAction)
                    hoverAction(it);
            }
            break;
        case KEY_LEFT:
        case 'a':
            if (!scroll && currentPosition % numColumns != 0)
            {
                menu_driver(menu, REQ_LEFT_ITEM);
                it--;
                if (hoverAction)
                    hoverAction(it);
            }
            break;
        case KEY_ENTER:
        case 10:
            selected = true;
            break;
        }
        wrefresh(menuWindow);
    }

    unpost_menu(menu);
    werase(menuWindow);
    wrefresh(menuWindow);
    delwin(menuSub);
    delwin(menuWindow);
    free_menu(menu);
    for (auto& item : items)
        free_item(item);

    return it->first;
}

void Screen::StartGame()
{
    m_View            = View::InGame;
    m_GameWorldWindow = newwin(1, 1, 0, 0);
    ResizeWorldWindow();
    m_GameHUDWindow     = newwin(ScreenHeight, HUDPanelWidth, 0, WorldPanelWidth);
    m_GameMessageWindow = newwin(ScreenHeight - WorldPanelHeight, WorldPanelWidth + 1, WorldPanelHeight, 0);
    m_Message           = "Welcome to the Dun-geon.";
}

void Screen::ResizeWorldWindow()
{
    werase(m_GameWorldWindow);
    wrefresh(m_GameWorldWindow);
    const Worlds::Room& currentRoom = m_WorldManager.CurrentRoom();

    Coords::Scalar windowLines   = currentRoom.GetHeight() + 2;
    Coords::Scalar windowColumns = currentRoom.GetWidth() + 2;

    if (currentRoom.GetCameraStyle() != CameraStyle::Fixed && currentRoom.GetVisionRadius() > 0)
    {
        windowLines   = currentRoom.GetVisionRadius() * 2 + 3;
        windowColumns = windowLines;
    }

    int windowXPos = (WorldPanelWidth - windowColumns) / 2 - 1;
    int windowYPos = (WorldPanelHeight - windowLines) / 2;
    wresize(m_GameWorldWindow, windowLines, windowColumns);
    mvwin(m_GameWorldWindow, windowYPos, windowXPos);
}

void Screen::DrawWorld()
{
    werase(m_GameWorldWindow);
    if (m_CurrentRoom != &m_WorldManager.CurrentRoom())
    {
        m_CurrentRoom = &m_WorldManager.CurrentRoom();
        ResizeWorldWindow();
    }
    int worldY, worldX;
    getmaxyx(m_GameWorldWindow, worldY, worldX);
    // How far can we draw vertically or horizontally
    Coords::Scalar rangeX = worldX / 2 - (worldX % 2 ? 0 : 1) - 1;
    Coords::Scalar rangeY = worldY / 2 - (worldY % 2 ? 0 : 1) - 1;
    auto playerCoords     = m_EntityManager.CoordsOf(m_Player);
    for (int i = 1; i < worldX - 1; i++)
    {
        for (int j = 1; j < worldY - 1; j++)
        {
            int desiredFieldXPos = 0;
            int desiredFieldYPos = 0;
            switch (m_CurrentRoom->GetCameraStyle())
            {
            case CameraStyle::Fixed:
                desiredFieldXPos = i - 1;
                desiredFieldYPos = j - 1;
                break;
            case CameraStyle::PlayerCentered:
                // These coords are relative to the player's position
                desiredFieldXPos = playerCoords.X + (i - 1) - rangeX;
                desiredFieldYPos = playerCoords.Y + (j - 1) - rangeY;
                break;
            }
            if (desiredFieldXPos < 0 || desiredFieldXPos >= m_CurrentRoom->GetWidth() || desiredFieldYPos < 0
                || desiredFieldYPos >= m_CurrentRoom->GetHeight())
            {
                mvwaddch(m_GameWorldWindow, j, i, DefaultFieldIcon);
            }
            else
            {
                auto radius = m_CurrentRoom->GetVisionRadius();
                Coords targetCoords(desiredFieldXPos, desiredFieldYPos);
                if (radius > 0
                    && playerCoords.Distance(targetCoords)
                           > (playerCoords.SharesAxis(targetCoords) ? radius - 1 : radius))
                {
                    mvwaddch(m_GameWorldWindow, j, i, DefaultFieldIcon);
                }
                else
                {
                    mvwaddch(m_GameWorldWindow, j, i, FieldIcon(targetCoords));
                }
            }
        }
    }
    if (m_CurrentRoom->GetCameraStyle() != CameraStyle::Fixed)
    {
        box(m_GameWorldWindow, 0, 0);
    }
    wrefresh(m_GameWorldWindow);
}

void Screen::DrawHUD()
{
    werase(m_GameHUDWindow);
    const auto& stats = m_Player.GetStats();
    mvwprintw(m_GameHUDWindow, 2, 4, "World %d", m_WorldManager.CurrentWorld().GetWorldNumber());
    mvwprintw(m_GameHUDWindow, 2, HUDPanelWidth - 10, "Room %d", m_WorldManager.CurrentRoom().GetRoomNumber());

    PrintCenter(m_GameHUDWindow, m_Player.GetName(), 4);

    mvwprintw(m_GameHUDWindow, 6, 4, "Level %d", stats.Level);
    Components::FillBar xpBar {
        m_GameHUDWindow, 14, 13, 6, m_Player.GetXP(), m_Player.GetXPToLevelUp(), ColorPairs::WhiteOnYellow
    };
    if (stats.Level != Entities::LevelCap)
    {
        xpBar.Draw();
    }

    mvwprintw(m_GameHUDWindow, 8, 4, "HP:  %d/%d", stats.Health, stats.MaxHealth);
    mvwprintw(m_GameHUDWindow, 8, HUDPanelWidth - 11, "(%3d%%)", PLAYER_HEALTH_PC);

    mvwprintw(m_GameHUDWindow, 9, 4, "MP:  %d/%d", stats.Mana, stats.MaxMana);
    mvwprintw(m_GameHUDWindow, 9, HUDPanelWidth - 11, "(%3d%%)", PLAYER_MANA_PC);

    wattron(m_GameHUDWindow, A_BOLD | COLOR_PAIR(ColorPairs::RedOnDefault));
    mvwprintw(m_GameHUDWindow, 11, 4, "Str: %3d", stats.Strength);
    wattron(m_GameHUDWindow, A_BOLD | COLOR_PAIR(ColorPairs::MagentaOnDefault));
    mvwprintw(m_GameHUDWindow, 11, HUDPanelWidth - 11, "Sor: %3d", stats.Sorcery);

    wattron(m_GameHUDWindow, A_BOLD | COLOR_PAIR(ColorPairs::GreenOnDefault));
    mvwprintw(m_GameHUDWindow, 12, 4, "Dex: %3d", stats.Dexterity);
    wattron(m_GameHUDWindow, A_BOLD | COLOR_PAIR(ColorPairs::BlueOnDefault));
    mvwprintw(m_GameHUDWindow, 12, HUDPanelWidth - 11, "Wis: %3d", stats.Wisdom);
    wattroff(m_GameHUDWindow, A_COLOR | A_BOLD);

    std::string wealthAmountStr = std::to_string(m_Player.GetDun());
    int xPos                    = (HUDPanelWidth - wealthAmountStr.size() - 12) / 2;
    xPos += xPos % 2;
    mvwaddstr(m_GameHUDWindow, 14, xPos, "Wealth: ");
    wattron(m_GameHUDWindow, COLOR_PAIR(ColorPairs::YellowOnDefault) | A_BOLD);
    wprintw(m_GameHUDWindow, "%d", m_Player.GetDun());
    wattroff(m_GameHUDWindow, A_COLOR | A_BOLD);
    waddstr(m_GameHUDWindow, " dun");

    mvwaddstr(m_GameHUDWindow, 16, 5, "[i]tems");
    mvwaddstr(m_GameHUDWindow, 16, HUDPanelWidth - 12, "[s]kills");

    mvwaddstr(m_GameHUDWindow, 17, 5, "[m]ap");
    mvwaddstr(m_GameHUDWindow, 17, HUDPanelWidth - 12, "[h]elp");

    PrintCenter(m_GameHUDWindow, "[q]uit", 18);

    auto approachedEntity = m_EntityManager.Approaching(m_Player, m_Player.FacingDirection);

    if (approachedEntity != nullptr)
    {
        PrintCenter(m_GameHUDWindow, approachedEntity->GetName(), WorldPanelHeight + 1);
        PrintCenter(m_GameHUDWindow, approachedEntity->GetDescription(), WorldPanelHeight + 2);
    }

    box(m_GameHUDWindow, 0, 0);
    mvwhline(m_GameHUDWindow, WorldPanelHeight, 1, 0, HUDPanelWidth - 2);
    mvwaddch(m_GameHUDWindow, WorldPanelHeight, HUDPanelWidth - 1, ACS_RTEE);
    wrefresh(m_GameHUDWindow);
}

void Screen::DrawMessageWindow(bool shouldPostMessage)
{
    werase(m_GameMessageWindow);
    wborder(m_GameMessageWindow, 0, 0, 0, 0, 0, ACS_PLUS, 0, ACS_BTEE);
    if (shouldPostMessage)
    {
        if (m_Message.size() > WorldPanelWidth - 4)
        {
            size_t pos = WorldPanelWidth - 5;
            while (m_Message[pos] != ' ' && pos > 0)
                pos--;
            std::string secondLine = m_Message.substr(pos > 0 ? pos + 1 : WorldPanelWidth - 4);
            m_Message              = m_Message.substr(0, pos > 0 ? pos : WorldPanelWidth - 4);
            mvwaddstr(m_GameMessageWindow, 1, 2, m_Message.c_str());
            mvwaddstr(m_GameMessageWindow, 2, 2, secondLine.c_str());
        }
        else
        {
            mvwaddstr(m_GameMessageWindow, 1, 2, m_Message.c_str());
        }
        m_Message.clear();
    }
    wrefresh(m_GameMessageWindow);
}

void Screen::DrawMap(WINDOW* mapWindow, Coords cursor)
{
    werase(mapWindow);
    wattron(mapWindow, COLOR_PAIR(ColorPairs::BlackOnYellow));
    box(mapWindow, 0, 0);
    PrintCenter(mapWindow, " [SPACE] to toggle cursor ", WorldMapHeight - 1);
    wattroff(mapWindow, A_COLOR);
    wattron(mapWindow, COLOR_PAIR(ColorPairs::WhiteOnYellow) | A_BOLD);
    PrintCenter(mapWindow, " World Map ", 0);
    wattroff(mapWindow, A_COLOR | A_BOLD);

    const auto& world = m_WorldManager.CurrentWorld();
    for (Coords::Scalar i = 0; i < Worlds::World::MaximumSpan; i++)
    {
        for (Coords::Scalar j = 0; j < Worlds::World::MaximumSpan; j++)
        {
            Coords current(i, j);
            WorldMapObjectType type = MapObjectType(current);

            // Select the icon based on the object type
            chtype icon;
            switch (type)
            {
            default:
            case WorldMapObjectType::Empty:
                icon = ' ';
                break;
            case WorldMapObjectType::Room:
            {
                const auto& room = world.RoomAt(current);
                icon             = RoomMapIcon(room);
                if (room.Entrance(Direction::Left) != nullptr)
                    mvwaddch(mapWindow, j + 1, i * 2, ACS_HLINE);
                if (room.Entrance(Direction::Right) != nullptr)
                    mvwaddch(mapWindow, j + 1, i * 2 + 2, ACS_HLINE);
                break;
            }
            case WorldMapObjectType::UndiscoveredRoom:
                icon = '?';
                break;
            }

            // Apply highlighting
            bool isCurrentRoom = m_WorldManager.CurrentRoom().GetCoords() == current;
            if (m_IsWorldMapCursorEnabled && cursor == current)
            {
                icon |= isCurrentRoom ? (COLOR_PAIR(ColorPairs::BlackOnRed)) : (COLOR_PAIR(ColorPairs::BlackOnYellow));
            }
            else if (isCurrentRoom)
            {
                icon |= (COLOR_PAIR(ColorPairs::RedOnDefault) | A_BOLD);
            }
            mvwaddch(mapWindow, j + 1, i * 2 + 1, icon);
        }
    }
    wrefresh(mapWindow);
}

void Screen::DrawMapTooltip(Coords cursor, WorldMapObjectType objectType)
{
    if (objectType == WorldMapObjectType::Empty)
        return;

    // Prepare window contents
    std::vector<std::string> lines;
    switch (objectType)
    {
    case WorldMapObjectType::Room:
    {
        const auto& room       = m_WorldManager.CurrentWorld().RoomAt(cursor);
        bool isCurrentRoom     = m_WorldManager.IsCurrentRoom(room);
        std::string locPronoun = isCurrentRoom ? "here" : "there";
        lines.push_back("Room " + std::to_string(room.GetRoomNumber()));
        if (m_WorldManager.IsCurrentRoom(room))
            lines.push_back("* You are here *");
        if (room.GetVisionRadius() > 0)
            lines.push_back("It's dark in " + locPronoun + ".");
        break;
    }
    case WorldMapObjectType::UndiscoveredRoom:
        lines.push_back("Undiscovered room");
        break;
    default:
        break;
    }

    // Scale the window
    int tooltipWidth = 0;
    for (const auto& line : lines)
        if (line.size() > static_cast<size_t>(tooltipWidth))
            tooltipWidth = line.size();
    tooltipWidth += 4;
    int tooltipHeight = lines.size() + 2;

    // Tooltip sticks to the cursor
    // Calculate the real coord on the screen first
    int cursorActualX = cursor.X * 2 + 1 + WorldMapXPos;
    int cursorActualY = cursor.Y + 1 + WorldMapYPos;
    WINDOW* tooltipWindow
        = newwin(tooltipHeight,
                 tooltipWidth,
                 // Flip the tooltip to the other side of the cursor
                 // if it cannot fit on either axis
                 cursorActualY > tooltipHeight ? cursorActualY - tooltipHeight : cursorActualY + 1,
                 cursorActualX < ScreenWidth - tooltipWidth - 1 ? cursorActualX + 1 : cursorActualX - tooltipWidth);
    // Draw the window
    wattron(tooltipWindow, COLOR_PAIR(ColorPairs::YellowOnDefault) | A_BOLD);
    box(tooltipWindow, 0, 0);
    wattroff(tooltipWindow, A_BOLD);
    for (size_t i = 0; i < lines.size(); i++)
    {
        PrintCenter(tooltipWindow, lines[i], i + 1);
    }
    wattroff(tooltipWindow, A_COLOR);
    wrefresh(tooltipWindow);

    // Cleanup
    delwin(tooltipWindow);
}

chtype Screen::FieldIcon(const Worlds::Field& field) const
{
    chtype icon           = 0;
    bool canHaveHighlight = true;
    if (field.ForegroundEntity() != nullptr)
    {
        icon = field.ForegroundEntity()->GetIcon();
    }
    else if (field.BackgroundEntity() != nullptr)
    {
        icon = field.BackgroundEntity()->GetIcon();
    }
    else if (field.IsAccessible() && m_CurrentRoom->GetVisionRadius() > 0)
    {
        icon             = '.' | COLOR_PAIR(ColorPairs::WhiteOnDefault);
        canHaveHighlight = false;
    }
    else
    {
        icon             = DefaultFieldIcon;
        canHaveHighlight = false;
    }

    // Apply highlight if the player is touching this field
    auto lmd          = m_Player.FacingDirection;
    auto playerCoords = m_EntityManager.CoordsOf(m_Player);
    if (canHaveHighlight && lmd != Direction::None && !m_CurrentRoom->IsAtRoomEdge(playerCoords, lmd)
        && field.GetCoords() == playerCoords.Adjacent(lmd))
    {
        short highlightPair;
        // Highlight the background only if it was a non-default color
        short bgColorPair = (icon & A_COLOR) >> 8;
        short fg, bg;
        pair_content(bgColorPair, &fg, &bg);
        highlightPair = (bg > 0) ? ColorPairs::RedOnRed : ColorPairs::RedOnDefault;

        icon &= ~A_COLOR;
        icon |= COLOR_PAIR(highlightPair) | A_BOLD;
    }

    return icon;
}

chtype Screen::FieldIcon(Coords coords) const
{
    return FieldIcon(m_WorldManager.CurrentRoom().FieldAt(coords));
}

chtype Screen::RoomMapIcon(const Worlds::Room& room) const
{
    bool up    = room.Entrance(Direction::Up) != nullptr;
    bool right = room.Entrance(Direction::Right) != nullptr;
    bool down  = room.Entrance(Direction::Down) != nullptr;
    bool left  = room.Entrance(Direction::Left) != nullptr;

    constexpr static const chtype deadEnd = '#';

    if (up)
    {
        if (right)
        {
            if (down && left)
                return ACS_PLUS;
            if (down && !left)
                return ACS_LTEE;
            if (!down && left)
                return ACS_BTEE;
            if (!down && !left)
                return ACS_LLCORNER;
        }
        else
        {
            if (down && left)
                return ACS_RTEE;
            if (down && !left)
                return ACS_VLINE;
            if (!down && left)
                return ACS_LRCORNER;
            if (!down && !left)
                return deadEnd;
        }
    }
    else
    {
        if (right)
        {
            if (down && left)
                return ACS_TTEE;
            if (down && !left)
                return ACS_ULCORNER;
            if (!down && left)
                return ACS_HLINE;
            if (!down && !left)
                return deadEnd;
        }
        else
        {
            if (down && left)
                return ACS_URCORNER;
            if (down && !left)
                return deadEnd;
            if (!down && left)
                return deadEnd;
        }
    }

    return deadEnd;
}

WorldMapObjectType Screen::MapObjectType(Coords coords) const
{
    WorldMapObjectType type = WorldMapObjectType::Empty;
    const auto& world       = m_WorldManager.CurrentWorld();
    if (!world.RoomExists(coords))
    {
        // If the room is undiscovered, we cannot access it directly, but we can check
        // if its neighbors have any entrances leading here.
        for (const auto& dir : Direction::All)
        {
            if (world.RoomExists(coords.Adjacent(dir))
                && world.RoomAt(coords.Adjacent(dir)).Entrance(dir.Opposite()) != nullptr)
            {
                type = WorldMapObjectType::UndiscoveredRoom;
                break;
            }
        }
    }
    else
    {
        type = WorldMapObjectType::Room;
    }

    return type;
}

} /* namespace UI */

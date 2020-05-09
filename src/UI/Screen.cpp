#include "Screen.h"
#include "CameraStyle.h"
#include "ColorPairs.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "InputHandler.h"
#include "Misc/Coords.h"
#include "Misc/Exceptions.h"
#include "Misc/RNG.h"
#include "Misc/Utils.h"
#include "Worlds/Field.h"
#include "Worlds/Generation/RoomLayout.h"
#include "Worlds/Room.h"
#include "Worlds/World.h"
#include "Worlds/WorldManager.h"
#include <cmath>
#include <iostream>
#include <map>
#include <menu.h>
#include <ncurses.h>
#include <sstream>
#include <string>

#define PLAYER_HEALTH_PC std::lround(m_Player.GetStats().health / 1.0 / m_Player.GetStats().healthMax * 100)
#define PLAYER_MANA_PC std::lround(m_Player.GetStats().mana / 1.0 / m_Player.GetStats().manaMax * 100)

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
      m_CurrentRoom(nullptr)
{
    Init();
}

Screen::~Screen()
{
    Terminate();
}

void Screen::MainMenu()
{
    m_View = View::MainMenu;
    static const std::vector<std::string> splashMsg = {
        "Speakest not of it.",
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
        "Hopefully it will have been worth the wait."
    };
    DrawLogo();
    int splashNumber = RNG::RandomInt(splashMsg.size());
    PrintCenterAt(splashMsg[splashNumber], 13);

    refresh();

    std::map<int, std::string> options = {
        { 0, "Start Game" },
        { 2, "Quit" }
    };
    static const int menuWidth = 20;
    static const int menuHeight = 9;
    int choice = SelectViaMenu(options, { (ScreenWidth - menuWidth) / 2 - 1, 15 }, menuWidth, menuHeight, false, 2, 1, "", true);
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
    m_View = View::Map;

    // Prepare window
    // Every room icon is 1 char wide and has 1 char of spacing on both sides horizontally
    // for drawing "hallways" between rooms. This also helps keep better proportions.
    int width = Worlds::World::MaximumSpan * 2 - 1 + 2;
    int height = Worlds::World::MaximumSpan + 2;
    WINDOW* mapWindow = newwin(height, width, (ScreenHeight - height) / 2, (ScreenWidth - width) / 2);
    wclear(mapWindow);
    box(mapWindow, 0, 0);
    wattron(mapWindow, A_REVERSE);
    PrintCenterAt(mapWindow, " World Map ", 0);
    wattroff(mapWindow, A_REVERSE);

    // Draw the map
    DrawMap(mapWindow);
    wgetch(mapWindow);

    // Clean up the window
    wclear(mapWindow);
    wrefresh(mapWindow);

    delwin(mapWindow);
    m_View = previousView;
}

bool Screen::YesNoMessageBox(const std::string& prompt, const std::string& leftOption, const std::string& rightOption, const std::string& title)
{
    // Split the prompt into lines
    std::vector<std::string> lines;
    size_t neededWidth = 0;
    std::string lineBuf;
    std::istringstream iss(prompt);
    while (std::getline(iss, lineBuf))
    {
        if (lineBuf.size() > neededWidth) neededWidth = lineBuf.size();
        lines.push_back(std::move(lineBuf));
    }

    std::string left = "  " + leftOption + "  ";
    std::string right = "  " + rightOption + "  ";
    // The ncurses menu forces all option boxes to be the same size.
    // Thus we should give the shorter option some padding
    std::string& longer = left.size() > right.size() ? left : right;
    std::string& shorter = left.size() > right.size() ? right : left;
    for (size_t i = 0; i < (longer.size() - shorter.size()) / 2; i++)
    {
        shorter.insert(shorter.begin(), ' ');
    }

    int height = lines.size() + 4;
    int width = neededWidth + 4;

    std::vector<ITEM*> items = { new_item(left.c_str(), left.c_str()),
                                 new_item(right.c_str(), right.c_str()),
                                 nullptr };

    int subXPos = (width - (left.size() + right.size() + 4)) / 2;

    WINDOW* boxWin = newwin(height, width, (ScreenHeight - height) / 2, (ScreenWidth - width) / 2);
    WINDOW* boxSub = derwin(boxWin, 1, width - subXPos - 3, height - 2, subXPos);
    MENU* menu = new_menu(items.data());

    set_menu_win(menu, boxWin);
    set_menu_sub(menu, boxSub);
    keypad(boxWin, 1);
    menu_opts_off(menu, O_SHOWDESC);
    set_menu_mark(menu, "");
    set_menu_format(menu, 1, 2);
    set_menu_spacing(menu, 1, 1, 4);
    box(boxWin, 0, 0);
    if (!title.empty()) PrintCenterAt(boxWin, title, 0);
    for (size_t i = 0; i < lines.size(); i++)
    {
        PrintCenterAt(boxWin, lines[i], i + 1);
    }

    post_menu(menu);
    wrefresh(boxWin);
    bool selectedLeft = true;

    bool pressedEnter = false;
    int key;
    while (!pressedEnter)
    {
        key = wgetch(boxWin);
        switch (key)
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

void Screen::Init()
{
    initscr();
    start_color();
    use_default_colors();
    raw();
    keypad(stdscr, true);
    noecho();
    curs_set(0);

    // Default color pairs
    init_pair(ColorPairs::Wall, -1, COLOR_WHITE);
    init_pair(ColorPairs::PlayerEntityIcon, COLOR_MAGENTA, -1);
    init_pair(ColorPairs::YellowText, COLOR_YELLOW, -1);
    init_pair(ColorPairs::WorldAccessibleField, COLOR_WHITE, -1);
    init_pair(ColorPairs::WorldTouchedField, COLOR_RED, COLOR_RED);
    init_pair(ColorPairs::WorldTouchedFieldNoBg, COLOR_RED, -1);
}

void Screen::Terminate()
{
    if (m_GameWorldWindow != nullptr) delwin(m_GameWorldWindow);
    if (m_GameHUDWindow != nullptr) delwin(m_GameHUDWindow);
    if (m_GameMessageWindow != nullptr) delwin(m_GameMessageWindow);
    endwin();
}

void Screen::PrintCenterAt(const std::string& str, int yPos)
{
    int xPos = (ScreenWidth - str.size()) / 2;
    xPos += xPos % 2;
    mvaddstr(yPos, xPos, str.c_str());
    refresh();
}

void Screen::PrintCenterAt(WINDOW* window, const std::string& str, int yPos)
{
    int xPos = (getmaxx(window) - str.size()) / 2;
    xPos += xPos % 2;
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
    printw("%d", GameVersionMinor);
    addch('.');
    attron(COLOR_PAIR(ColorPairs::YellowText));
    printw("%d", GameVersionRevision);
    attroff(A_COLOR);
    attroff(A_BOLD);
}

int Screen::SelectViaMenu(std::map<int, std::string> options, Coords position, int width, int height, bool drawBorder, int padX, int padY, const std::string& title, bool spaceOptions)
{
    if (options.empty())
    {
        std::ostringstream errorMessage;
        errorMessage << "Attempted display of empty "
                     << width << 'x' << height
                     << " menu at "
                     << position;
        if (!title.empty()) errorMessage << " (\"" << title << "\")";
        throw DisplayException(errorMessage.str());
    }

    const size_t subWidth = width - 2 - 2 * padX;

    std::vector<ITEM*> items;
    for (auto& pair : options)
    {
        if (pair.second.size() + 4 > subWidth)
        {
            ShortenString(pair.second, subWidth - 4);
        }
        pair.second = std::string("  ") + pair.second + "  ";
        items.push_back(new_item(pair.second.c_str(), pair.second.c_str()));
    }
    items.push_back(nullptr);
    MENU* menu = new_menu(items.data());
    WINDOW* menuWindow = newwin(height,
                                width,
                                position.GetY(),
                                position.GetX());
    WINDOW* menuSub = derwin(menuWindow,
                             height - 2 - 2 * padY,
                             subWidth,
                             1 + padY,
                             1 + padX);
    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, menuSub);
    keypad(menuWindow, 1);

    menu_opts_off(menu, O_SHOWDESC);
    set_menu_mark(menu, "");
    if (spaceOptions) set_menu_spacing(menu, 1, 2, 1);
    if (drawBorder) box(menuWindow, 0, 0);
    if (!title.empty()) PrintCenterAt(menuWindow, title, 0);

    post_menu(menu);
    wrefresh(menuWindow);

    auto it = options.begin();
    bool selected = false;
    int key;
    while (!selected)
    {
        key = wgetch(menuWindow);
        switch (key)
        {
        case KEY_DOWN:
        case 's':
            menu_driver(menu, REQ_DOWN_ITEM);
            if (++it == options.end())
            {
                menu_driver(menu, REQ_FIRST_ITEM);
                it = options.begin();
            }
            break;
        case KEY_UP:
        case 'w':
            menu_driver(menu, REQ_UP_ITEM);
            if (it == options.begin())
            {
                menu_driver(menu, REQ_LAST_ITEM);
                it = options.end();
            }
            it--;
            break;
        case KEY_ENTER:
        case 10:
            selected = true;
            break;
        }
        wrefresh(menuWindow);
    }

    unpost_menu(menu);
    wclear(menuWindow);
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
    m_View = View::InGame;
    m_GameWorldWindow = newwin(1, 1, 0, 0);
    ResizeAndRepositionWorldWindow();
    m_GameHUDWindow = newwin(ScreenHeight, HUDPanelWidth, 0, WorldPanelWidth);
    m_GameMessageWindow = newwin(ScreenHeight - WorldPanelHeight, WorldPanelWidth + 1, WorldPanelHeight, 0);
    m_Message = "Welcome to the Dun-geon.";
}

void Screen::ResizeAndRepositionWorldWindow()
{
    wclear(m_GameWorldWindow);
    wrefresh(m_GameWorldWindow);
    const Worlds::Room& currentRoom = m_WorldManager.GetCurrentRoom();

    Coords::Scalar windowLines = currentRoom.GetHeight() + 2;
    Coords::Scalar windowColumns = currentRoom.GetWidth() + 2;

    if (currentRoom.GetCameraStyle() != CameraStyle::Fixed &&
        currentRoom.GetVisionRadius() > 0)
    {
        windowLines = currentRoom.GetVisionRadius() * 2 + 3;
        windowColumns = windowLines;
    }

    int windowXPos = (WorldPanelWidth - windowColumns) / 2 - 1;
    int windowYPos = (WorldPanelHeight - windowLines) / 2;
    wresize(m_GameWorldWindow, windowLines, windowColumns);
    mvwin(m_GameWorldWindow, windowYPos, windowXPos);
}

void Screen::DrawWorld()
{
    wclear(m_GameWorldWindow);
    if (m_CurrentRoom != &m_WorldManager.GetCurrentRoom())
    {
        m_CurrentRoom = &m_WorldManager.GetCurrentRoom();
        ResizeAndRepositionWorldWindow();
    }
    int worldY, worldX;
    getmaxyx(m_GameWorldWindow, worldY, worldX);
    // How far can we draw vertically or horizontally
    Coords::Scalar rangeX = worldX / 2 - (worldX % 2 ? 0 : 1) - 1;
    Coords::Scalar rangeY = worldY / 2 - (worldY % 2 ? 0 : 1) - 1;
    auto playerCoords = m_Player.GetCoords();
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
                desiredFieldXPos = playerCoords.GetX() + (i - 1) - rangeX;
                desiredFieldYPos = playerCoords.GetY() + (j - 1) - rangeY;
                break;
            }
            if (desiredFieldXPos < 0 ||
                desiredFieldXPos >= m_CurrentRoom->GetWidth() ||
                desiredFieldYPos < 0 ||
                desiredFieldYPos >= m_CurrentRoom->GetHeight())
            {
                mvwaddch(m_GameWorldWindow, j, i, DefaultFieldIcon);
            }
            else
            {
                auto radius = m_CurrentRoom->GetVisionRadius();
                Coords targetCoords(desiredFieldXPos, desiredFieldYPos);
                if (radius > 0 &&
                    playerCoords.CombinedDistanceFrom(targetCoords) >
                        (playerCoords.SharesAxisWith(targetCoords)
                             ? radius - 1
                             : radius))
                {
                    mvwaddch(m_GameWorldWindow, j, i, DefaultFieldIcon);
                }
                else
                {
                    mvwaddch(m_GameWorldWindow, j, i, GetFieldIcon(targetCoords));
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
    wclear(m_GameHUDWindow);
    const auto& stats = m_Player.GetStats();
    mvwprintw(m_GameHUDWindow, 2, 4, "World %d", m_WorldManager.GetCurrentWorld().GetWorldNumber());
    mvwprintw(m_GameHUDWindow, 2, HUDPanelWidth - 10, "Room %d", m_WorldManager.GetCurrentRoom().GetRoomNumber());

    PrintCenterAt(m_GameHUDWindow, m_Player.GetName(), 4);

    mvwprintw(m_GameHUDWindow, 6, 4, "Level %d", stats.level);
    mvwprintw(m_GameHUDWindow, 6, HUDPanelWidth - 11, "XP: %3d%%", stats.XP / stats.XPToNextLevel * 100);

    mvwprintw(m_GameHUDWindow, 8, 4, "HP:  %d/%d", stats.health, stats.healthMax);
    mvwprintw(m_GameHUDWindow, 8, HUDPanelWidth - 11, "(%3d%%)", PLAYER_HEALTH_PC);

    mvwprintw(m_GameHUDWindow, 9, 4, "MP:  %d/%d", stats.mana, stats.manaMax);
    mvwprintw(m_GameHUDWindow, 9, HUDPanelWidth - 11, "(%3d%%)", PLAYER_MANA_PC);

    mvwprintw(m_GameHUDWindow, 11, 4, "Str: %3d", stats.vigor);
    mvwprintw(m_GameHUDWindow, 11, HUDPanelWidth - 11, "Def: %3d", stats.valor);

    mvwprintw(m_GameHUDWindow, 12, 4, "Agi: %3d", stats.haste);
    mvwprintw(m_GameHUDWindow, 12, HUDPanelWidth - 11, "Int: %3d", stats.magic);

    std::string wealthAmountStr = std::to_string(stats.dun);
    int xPos = (HUDPanelWidth - wealthAmountStr.size() - 12) / 2;
    xPos += xPos % 2;
    mvwaddstr(m_GameHUDWindow, 14, xPos, "Wealth: ");
    wattron(m_GameHUDWindow, COLOR_PAIR(ColorPairs::YellowText) | A_BOLD);
    wprintw(m_GameHUDWindow, "%d", stats.dun);
    wattroff(m_GameHUDWindow, A_COLOR | A_BOLD);
    waddstr(m_GameHUDWindow, " dun");

    mvwaddstr(m_GameHUDWindow, 16, 5, "[i]tems");
    mvwaddstr(m_GameHUDWindow, 16, HUDPanelWidth - 12, "[s]kills");

    mvwaddstr(m_GameHUDWindow, 17, 5, "[m]ap");
    mvwaddstr(m_GameHUDWindow, 17, HUDPanelWidth - 12, "[h]elp");

    PrintCenterAt(m_GameHUDWindow, "[q]uit", 18);

    if (m_EntityManager.GetApproachedEntity(m_Player) != nullptr)
    {
        PrintCenterAt(m_GameHUDWindow, m_EntityManager.GetApproachedEntity(m_Player)->GetName(), WorldPanelHeight + 1);
        PrintCenterAt(m_GameHUDWindow, m_EntityManager.GetApproachedEntity(m_Player)->GetDescription(), WorldPanelHeight + 2);
    }

    box(m_GameHUDWindow, 0, 0);
    mvwhline(m_GameHUDWindow, WorldPanelHeight, 1, 0, HUDPanelWidth - 2);
    mvwaddch(m_GameHUDWindow, WorldPanelHeight, HUDPanelWidth - 1, ACS_RTEE);
    wrefresh(m_GameHUDWindow);
}

void Screen::DrawMessageWindow()
{
    wclear(m_GameMessageWindow);
    wborder(m_GameMessageWindow, 0, 0, 0, 0, 0, ACS_PLUS, 0, ACS_BTEE);
    // Post the current message, split it into two lines if it's too long
    if (m_Message.size() > WorldPanelWidth - 4)
    {
        size_t pos = WorldPanelWidth - 5;
        while (m_Message[pos] != ' ' && pos > 0)
            pos--;
        std::string secondLine = m_Message.substr(pos > 0 ? pos + 1 : WorldPanelWidth - 4);
        m_Message = m_Message.substr(0, pos > 0 ? pos : WorldPanelWidth - 4);
        mvwaddstr(m_GameMessageWindow, 1, 2, m_Message.c_str());
        mvwaddstr(m_GameMessageWindow, 2, 2, secondLine.c_str());
    }
    else
    {
        mvwaddstr(m_GameMessageWindow, 1, 2, m_Message.c_str());
    }
    m_Message.clear();
    wrefresh(m_GameMessageWindow);
}

void Screen::DrawMap(WINDOW* mapWindow)
{
    const auto& world = m_WorldManager.GetCurrentWorld();
    for (Coords::Scalar i = 0; i < Worlds::World::MaximumSpan; i++)
    {
        for (Coords::Scalar j = 0; j < Worlds::World::MaximumSpan; j++)
        {
            chtype icon;
            Coords current(i, j);
            if (!world.RoomExistsAt(current))
            {
                icon = ' ';
                for (const auto& dir : Direction::All())
                {

                    if (world.RoomExistsAt(current.GetAdjacent(dir)) &&
                        world.GetRoomAt(current.GetAdjacent(dir)).TryGetEntrance(dir.Opposite()) != nullptr)
                    {
                        icon = '?';
                        break;
                    }
                }
            }
            else
            {
                const auto& room = world.GetRoomAt(current);
                icon = GetRoomMapIcon(room);
                if (room.TryGetEntrance(Direction::Left()) != nullptr)
                    mvwaddch(mapWindow, j + 1, i * 2, ACS_HLINE);
                if (room.TryGetEntrance(Direction::Right()) != nullptr)
                    mvwaddch(mapWindow, j + 1, i * 2 + 2, ACS_HLINE);
            }
            if (m_WorldManager.GetCurrentRoom().GetCoords() == current)
            {
                icon |= (COLOR_PAIR(ColorPairs::WorldTouchedFieldNoBg) | A_BOLD);
            }
            mvwaddch(mapWindow, j + 1, i * 2 + 1, icon);
        }
    }
    wrefresh(mapWindow);
}

chtype Screen::GetFieldIcon(const Worlds::Field& field) const
{
    chtype icon = 0;
    bool canHaveHighlight = true;
    if (field.TryGetForegroundEntity() != nullptr)
    {
        icon = field.TryGetForegroundEntity()->GetIcon();
    }
    else if (field.TryGetBackgroundEntity() != nullptr)
    {
        icon = field.TryGetBackgroundEntity()->GetIcon();
    }
    else if (field.IsAccessible() && m_CurrentRoom->GetVisionRadius() > 0)
    {
        icon = '.' | COLOR_PAIR(ColorPairs::WorldAccessibleField);
        canHaveHighlight = false;
    }
    else
    {
        icon = DefaultFieldIcon;
        canHaveHighlight = false;
    }

    // Apply highlight if the player is touching this field
    auto lmd = m_Player.GetLastMoveDirection();
    if (canHaveHighlight &&
        lmd != Direction::None() &&
        !m_CurrentRoom->IsPositionAtRoomEdge(m_Player.GetCoords(), lmd) &&
        field.GetCoords() == m_Player.GetCoords().GetAdjacent(lmd))
    {
        short highlightPair;
        // Highlight the background only if it was a non-default color
        short bgColorPair = (icon & A_COLOR) >> 8;
        short fg, bg;
        pair_content(bgColorPair, &fg, &bg);
        highlightPair = (bg > 0) ? ColorPairs::WorldTouchedField : ColorPairs::WorldTouchedFieldNoBg;

        icon &= ~A_COLOR;
        icon |= COLOR_PAIR(highlightPair) | A_BOLD;
    }

    return icon;
}

chtype Screen::GetFieldIcon(Coords coords) const
{
    return GetFieldIcon(m_WorldManager.GetCurrentRoom().GetFieldAt(coords));
}

chtype Screen::GetRoomMapIcon(const Worlds::Room& room) const
{
    bool up = room.TryGetEntrance(Direction::Up()) != nullptr;
    bool right = room.TryGetEntrance(Direction::Right()) != nullptr;
    bool down = room.TryGetEntrance(Direction::Down()) != nullptr;
    bool left = room.TryGetEntrance(Direction::Left()) != nullptr;

    if (up && right && down && left) return ACS_PLUS;
    if (up && right && down && !left) return ACS_LTEE;
    if (up && right && !down && left) return ACS_BTEE;
    if (up && right && !down && !left) return ACS_LLCORNER;
    if (up && !right && down && left) return ACS_RTEE;
    if (up && !right && down && !left) return ACS_VLINE;
    if (up && !right && !down && left) return ACS_LRCORNER;
    if (up && !right && !down && !left) return ACS_DIAMOND;
    if (!up && right && down && left) return ACS_TTEE;
    if (!up && right && down && !left) return ACS_ULCORNER;
    if (!up && right && !down && left) return ACS_HLINE;
    if (!up && right && !down && !left) return ACS_DIAMOND;
    if (!up && !right && down && left) return ACS_URCORNER;
    if (!up && !right && down && !left) return ACS_DIAMOND;
    if (!up && !right && !down && left) return ACS_DIAMOND;
    return 'x';
}

} /* namespace UI */
#include "Screen.h"
#include "ColorPairs.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Coords.h"
#include "Misc/Exceptions.h"
#include "Misc/RNG.h"
#include "Misc/Utils.h"
#include "InputHandler.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include "Worlds/World.h"
#include "Worlds/WorldManager.h"
#include <cmath>
#include <iostream>
#include <limits>
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
      m_GameMessageWindow(nullptr)
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
        "I dun my robe and wizard hat."
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
        m_InputHandler.setQuit();
        break;
    }
}

chtype Screen::GetFieldIcon(const Worlds::Field& field) const
{
    if (field.TryGetForegroundEntity() != nullptr)
    {
        return field.TryGetForegroundEntity()->GetIcon();
    }
    else if (field.TryGetBackgroundEntity() != nullptr)
    {
        return field.TryGetBackgroundEntity()->GetIcon();
    }
    else
    {
        return ' ';
    }
}

chtype Screen::GetFieldIcon(Coords coords) const
{
    return GetFieldIcon(m_WorldManager.GetCurrentRoom().GetFieldAt(coords));
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
}

void Screen::Draw()
{
    if (m_CurrentRoom != &m_WorldManager.GetCurrentRoom())
    {
        m_CurrentRoom = &m_WorldManager.GetCurrentRoom();
        ResizeAndRepositionWorldWindow();
    }
    size_t worldY, worldX;
    getmaxyx(m_GameWorldWindow, worldY, worldX);
    for (size_t i = 1; i < worldX - 1; i++)
    {
        for (size_t j = 1; j < worldY - 1; j++)
        {
            mvwaddch(m_GameWorldWindow, j, i, GetFieldIcon({ i - 1, j - 1 }));
        }
    }
    //wborder(m_GameWorldWindow, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(m_GameWorldWindow);

    DrawHUD();
    box(m_GameHUDWindow, 0, 0);
    mvwhline(m_GameHUDWindow, WorldPanelHeight, 1, 0, HUDPanelWidth - 2);
    mvwaddch(m_GameHUDWindow, WorldPanelHeight, HUDPanelWidth - 1, ACS_RTEE);
    wrefresh(m_GameHUDWindow);

    wclear(m_GameMessageWindow);
    wborder(m_GameMessageWindow, 0, 0, 0, 0, 0, ACS_PLUS, 0, ACS_BTEE);
    mvwaddstr(m_GameMessageWindow, 1, 1, m_InputHandler.getMessage().c_str());
    wrefresh(m_GameMessageWindow);
}

Screen::View Screen::GetView() const
{
    return m_View;
}

void Screen::Init()
{
    initscr();
    start_color();
    use_default_colors();
    cbreak();
    keypad(stdscr, true);
    noecho();
    curs_set(0);

    // Default color pairs
    init_pair(ColorPairs::Wall, -1, COLOR_WHITE);
    init_pair(ColorPairs::PlayerEntityIcon, COLOR_MAGENTA, -1);
    init_pair(ColorPairs::YellowText, COLOR_YELLOW, -1);
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
    mvaddstr(++yPos, xPos, "_____");
    mvaddstr(++yPos, xPos, "|    \\   _   _   _  __         ____   ____   ____   _  __");
    mvaddstr(++yPos, xPos, "| /\\  \\ / \\ / \\ | |/  \\  ___  /    \\ / __ \\ /    \\ | |/  \\");
    mvaddstr(++yPos, xPos, "| \\/  / | |_| | |  _  | |___| | () | | ___/ | () | |  _  |");
    mvaddstr(++yPos, xPos, "\\____/  \\_____| \\_/ \\_|       \\__  | \\____/ \\____/ \\_/ \\_|");
    mvaddstr(++yPos, xPos + 31, "__| |");
    mvprintw(++yPos, xPos + 30, "|____/  %s",
             GameVersionString.c_str());
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
    keypad(menuWindow, true);
    set_menu_win(menu, menuWindow);
    set_menu_sub(menu, menuSub);

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
    delwin(menuSub);
    delwin(menuWindow);
    free_menu(menu);
    for (auto& item : items)
        free_item(item);

    return it->first;
}

void Screen::StartGame()
{
    m_View = View::World;
    m_GameWorldWindow = newwin(1, 1, 0, 0);
    ResizeAndRepositionWorldWindow();
    m_GameHUDWindow = newwin(ScreenHeight, HUDPanelWidth, 0, WorldPanelWidth);
    m_GameMessageWindow = newwin(ScreenHeight - WorldPanelHeight, WorldPanelWidth + 1, WorldPanelHeight, 0);
}

void Screen::ResizeAndRepositionWorldWindow()
{
    wclear(m_GameWorldWindow);
    wrefresh(m_GameWorldWindow);
    const Worlds::Room& currentRoom = m_WorldManager.GetCurrentRoom();
    const Coords WorldWindowPos = { (WorldPanelWidth - currentRoom.GetWidth() - 2) / 2 - 1,
                                    (WorldPanelHeight - currentRoom.GetHeight() - 2) / 2 };
    wresize(m_GameWorldWindow, currentRoom.GetHeight() + 2, currentRoom.GetWidth() + 2);
    mvwin(m_GameWorldWindow, WorldWindowPos.GetY(), WorldWindowPos.GetX());
}

} /* namespace UI */
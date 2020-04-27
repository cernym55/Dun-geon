#include "Screen.h"
#include "Application/Application.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Coords.h"
#include "Misc/Exceptions.h"
#include "Misc/RNG.h"
#include "Misc/Utils.h"
#include "Parser.h"
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

Screen::Screen(Parser& parser,
               const Worlds::WorldManager& worldManager,
               const Entities::EntityManager& entityManager,
               const Entities::Player& player)
    : m_Parser(parser),
      m_WorldManager(worldManager),
      m_EntityManager(entityManager),
      m_Player(player)
{
    Init();
}

Screen::~Screen()
{
    Terminate();
}

Parser& Screen::GetParser()
{
    return m_Parser;
}

void Screen::PrintCenter(std::string str, int spaceWidth, bool secondPad)
{
    int pad = (spaceWidth - str.size());
    pad = (pad - pad % 2) / 2;
    int pad1 = pad;

    // if result width and desired width are off by 1, extend first pad
    if ((pad * 2 + str.size()) % 2 != spaceWidth % 2)
    {
        pad1++;
    }
    for (int i = 0; i < pad1; i++)
    {
        addch(' ');
    }
    addstr(str.c_str());
    if (secondPad)
    {
        for (int i = 0; i < pad; i++)
        {
            addch(' ');
        }
    }
}

void Screen::MainMenu()
{
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
        // begin game
        break;
    case 2:
        GetParser().setQuit();
        break;
    }
}

char Screen::GetFieldIcon(const Worlds::Field& field) const
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

char Screen::GetFieldIcon(Coords coords) const
{
    return GetFieldIcon(m_WorldManager.GetCurrentRoom().GetFieldAt(coords));
}

std::string Screen::GetMapRow(int rowNumber)
{
    if (rowNumber > m_WorldManager.GetCurrentRoom().GetHeight() - 1)
    {
        return "";
    }
    else
    {
        std::string mapRow = "";
        for (int i = 0; i < m_WorldManager.GetCurrentRoom().GetWidth(); i++)
        {
            mapRow += GetFieldIcon({ i, rowNumber });
        }
        return mapRow;
    }
}

void Screen::PrintHUDRow(int rowNumber)
{
    const auto& stats = m_Player.GetStats();
    int i;
    switch (rowNumber)
    {
    case 0:
    case 18:
    case 21:
        std::cout << '+';
        for (i = 0; i < HUDPanelWidth - 2; i++)
        {
            std::cout << '-';
        }
        std::cout << "+\n";
        break;
    // blank line
    case 1:
    case 3:
    case 8:
    case 11:
    case 13:
    case 17:
        std::cout << '|';
        for (i = 0; i < HUDPanelWidth - 2; i++)
        {
            std::cout << ' ';
        }
        std::cout << "|\n";
        break;
    case 2:
        std::cout << "|   World " << m_WorldManager.GetCurrentWorld().GetWorldNumber();
        for (i = 0; i < 10 - std::to_string(m_WorldManager.GetCurrentRoom().GetRoomNumber()).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "Room " << m_WorldManager.GetCurrentRoom().GetRoomNumber() << "   |\n";
        break;
    case 4:
        std::cout << '|';
        PrintCenter(m_Player.GetName(), HUDPanelWidth - 2, true);
        std::cout << "|\n";
        break;
    case 5:
        std::cout << "|   Level " << stats.level;
        for (i = 0; i < 11 - std::to_string(stats.level).size() - std::to_string(stats.XP).size() - std::to_string(stats.XPToNextLevel).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "XP: " << stats.XP << '/' << stats.XPToNextLevel << "   |\n";
        break;
    case 6:
        std::cout << "|   HP: " << stats.health << '/' << stats.healthMax << " ("
                  << PLAYER_HEALTH_PC << "%)";
        for (i = 0; i < 16 - std::to_string(stats.health).size() - std::to_string(stats.healthMax).size() - std::to_string(PLAYER_HEALTH_PC).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "|\n";
        break;
    case 7:
        std::cout << "|   MP: " << stats.mana << '/' << stats.manaMax << " ("
                  << PLAYER_MANA_PC << "%)";
        for (i = 0; i < 16 - std::to_string(stats.mana).size() - std::to_string(stats.manaMax).size() - std::to_string(PLAYER_MANA_PC).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "|\n";
        break;
    case 9:
        std::cout << "|   Vigor: " << stats.vigor;
        for (i = 0; i < 8 - std::to_string(stats.vigor).size() - std::to_string(stats.valor).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "Valor: " << stats.valor << "   |\n";
        break;
    case 10:
        std::cout << "|   Haste: " << stats.haste;
        for (i = 0; i < 8 - std::to_string(stats.haste).size() - std::to_string(stats.magic).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "Magic: " << stats.magic << "   |\n";
        break;
    case 12:
        std::cout << '|';
        PrintCenter("Wealth: " + std::to_string(stats.dun) + " dun", HUDPanelWidth - 2, true);
        std::cout << "|\n";
        break;
    case 14:
        std::cout << '|';
        PrintCenter("[i]tems", HUDPanelWidth / 2 - 1, true);
        PrintCenter("[s]kills", HUDPanelWidth / 2 - 1, true);
        std::cout << "|\n";
        break;
    case 15:
        std::cout << '|';
        PrintCenter("[m]ap ", HUDPanelWidth / 2 - 1, true);
        PrintCenter("[h]elp  ", HUDPanelWidth / 2 - 1, true);
        std::cout << "|\n";
        break;
    case 16:
        std::cout << '|';
        PrintCenter("[q]uit", HUDPanelWidth - 2, true);
        std::cout << "|\n";
        break;
    case 19:
        std::cout << '|';
        if (m_EntityManager.GetApproachedEntity(m_Player) != nullptr)
        {
            PrintCenter(m_EntityManager.GetApproachedEntity(m_Player)->GetName(), HUDPanelWidth - 2, true);
        }
        else
        {
            for (int i = 0; i < HUDPanelWidth - 2; i++)
            {
                std::cout << ' ';
            }
        }
        std::cout << "|\n";
        break;
    case 20:
        std::cout << '|';
        if (m_EntityManager.GetApproachedEntity(m_Player) != nullptr)
        {
            PrintCenter(m_EntityManager.GetApproachedEntity(m_Player)->GetDescription(), HUDPanelWidth - 2, true);
        }
        else
        {
            for (int i = 0; i < HUDPanelWidth - 2; i++)
            {
                std::cout << ' ';
            }
        }
        std::cout << "|\n";
        break;
    }
}

void Screen::Draw()
{
    // line 1
    Clear();
    for (int i = 0; i < MapPanelWidth; i++)
    {
        std::cout << ' ';
    }
    PrintHUDRow(0);

    // ...and the rest (getMapRow returns blank string if passed invalid row)
    for (int i = 1; i < 22; i++)
    {
        PrintCenter(GetMapRow(i - 1), MapPanelWidth, true);
        PrintHUDRow(i);
    }

    // prompt separator
    std::cout << '+';
    for (int i = 0; i < ScreenWidth - 2; i++)
    {
        std::cout << '-';
    }
    std::cout << "+\n";

    // message + prompt
    std::cout << "  " << GetParser().getMessage() << "\n  > ";
}

Screen::View Screen::GetView()
{
    return m_View;
}

void Screen::SetView(View m)
{
    m_View = m;
}

void Screen::Clear()
{
    std::cout << "\033[2J\033[;H";
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
}

void Screen::Terminate()
{
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

} /* namespace UI */
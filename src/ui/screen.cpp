#include "screen.h"
#include "application/application.h"
#include "entities/player.h"
#include "misc/utils.h"
#include "parser.h"
#include "worlds/room.h"
#include "worlds/world.h"
#include <cmath>
#include <limits>

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 24
#define MAP_PANEL_SIZE 50
#define HUD_PANEL_SIZE 30
#define PLAYER_STATS m_CurrentRoom->getParentWorld()->GetPlayer()->getStats()
#define PLAYER_HEALTH_PC std::lround(PLAYER_STATS.health / 1.0 / PLAYER_STATS.healthMax * 100)
#define PLAYER_MANA_PC std::lround(PLAYER_STATS.mana / 1.0 / PLAYER_STATS.manaMax * 100)

namespace UI
{

Screen::Screen(Parser& parser, Worlds::WorldManager& worldManager)
    : m_Parser(parser), m_WorldManager(worldManager)
{
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
        std::cout << ' ';
    }
    std::cout << str;
    if (secondPad)
    {
        for (int i = 0; i < pad; i++)
        {
            std::cout << ' ';
        }
    }
}

void Screen::MainMenu()
{
    std::vector<std::string> splashMsg = {
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
    std::cout << "\n\n\n\n\n           "
              << "_____\n           "
              << "|    \\   _   _   _  __         ____   ____   ____   _  __\n           "
              << "| /\\  \\ / \\ / \\ | |/  \\  ___  /    \\ / __ \\ /    \\ | |/  \\\n           "
              << "| \\/  / | |_| | |  _  | |___| | () | | ___/ | () | |  _  |\n           "
              << "\\____/  \\_____| \\_/ \\_|       \\__  | \\____/ \\____/ \\_/ \\_|\n           "
              << "                               __| |\n           "
              << "                              |____/\t  by Matt Black\n\n\n";
    int splashNumber = RNG(0, splashMsg.size() - 1);
    PrintCenter(splashMsg[splashNumber], WINDOW_WIDTH, false);
    std::cout << "\n\n\n";
    PrintCenter("Select an option:", WINDOW_WIDTH, false);
    std::cout << std::endl;
    PrintCenter("b - [b]egin", WINDOW_WIDTH, false);
    std::cout << std::endl;
    PrintCenter("h - [h]elp", WINDOW_WIDTH, false);
    std::cout << std::endl;
    PrintCenter("q - [q]uit", WINDOW_WIDTH, false);
    std::cout << "\n\n\n\t\t\t> ";
    char select;
    std::cin >> select;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (select)
    {
    case 'b':
    case 'B':
        // begin game
        break;
    case 'h':
    case 'H':
        // help
        break;
    case 'q':
    case 'Q':
        GetParser().setQuit();
        break;
    }
}

char Screen::GetFieldIcon(int fieldX, int fieldY)
{
    char fieldIcon;
    if (m_CurrentRoom->getField(fieldX, fieldY)->isWall)
    {
        fieldIcon = '#'; // if wall
    }
    else if (m_CurrentRoom->getField(fieldX, fieldY)->content == nullptr)
    {
        fieldIcon = ' '; // blank space if not wall nor entity (empty)
    }
    else
    {
        fieldIcon = m_CurrentRoom->getField(fieldX, fieldY)->content->getIcon(); // else get icon from entity
    }
    return fieldIcon;
}

std::string Screen::GetMapRow(int rowNumber)
{
    if (rowNumber > m_CurrentRoom->getDimY() - 1)
    {
        return "";
    }
    else
    {
        std::string mapRow = "";
        for (int i = 0; i < m_CurrentRoom->getDimX(); i++)
        {
            mapRow += GetFieldIcon(i, rowNumber);
        }
        return mapRow;
    }
}

void Screen::PrintHUDRow(int rowNumber)
{
    int i;
    switch (rowNumber)
    {
    case 0:
    case 18:
    case 21:
        std::cout << '+';
        for (i = 0; i < HUD_PANEL_SIZE - 2; i++)
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
        for (i = 0; i < HUD_PANEL_SIZE - 2; i++)
        {
            std::cout << ' ';
        }
        std::cout << "|\n";
        break;
    case 2:
        std::cout << "|   World " << m_CurrentRoom->getParentWorld()->GetWorldNumber();
        for (i = 0; i < 10 - std::to_string(m_CurrentRoom->getRoomNum()).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "Room " << m_CurrentRoom->getRoomNum() << "   |\n";
        break;
    case 4:
        std::cout << '|';
        PrintCenter(m_CurrentRoom->getParentWorld()->GetPlayer()->getName(), HUD_PANEL_SIZE - 2, true);
        std::cout << "|\n";
        break;
    case 5:
        std::cout << "|   Level " << PLAYER_STATS.level;
        for (i = 0; i < 11 - std::to_string(PLAYER_STATS.level).size() - std::to_string(PLAYER_STATS.XP).size() - std::to_string(PLAYER_STATS.XPToNextLevel).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "XP: " << PLAYER_STATS.XP << '/' << PLAYER_STATS.XPToNextLevel << "   |\n";
        break;
    case 6:
        std::cout << "|   HP: " << PLAYER_STATS.health << '/' << PLAYER_STATS.healthMax << " ("
                  << PLAYER_HEALTH_PC << "%)";
        for (i = 0; i < 16 - std::to_string(PLAYER_STATS.health).size() - std::to_string(PLAYER_STATS.healthMax).size() - std::to_string(PLAYER_HEALTH_PC).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "|\n";
        break;
    case 7:
        std::cout << "|   MP: " << PLAYER_STATS.mana << '/' << PLAYER_STATS.manaMax << " ("
                  << PLAYER_MANA_PC << "%)";
        for (i = 0; i < 16 - std::to_string(PLAYER_STATS.mana).size() - std::to_string(PLAYER_STATS.manaMax).size() - std::to_string(PLAYER_MANA_PC).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "|\n";
        break;
    case 9:
        std::cout << "|   Vigor: " << PLAYER_STATS.vigor;
        for (i = 0; i < 8 - std::to_string(PLAYER_STATS.vigor).size() - std::to_string(PLAYER_STATS.valor).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "Valor: " << PLAYER_STATS.valor << "   |\n";
        break;
    case 10:
        std::cout << "|   Haste: " << PLAYER_STATS.haste;
        for (i = 0; i < 8 - std::to_string(PLAYER_STATS.haste).size() - std::to_string(PLAYER_STATS.magic).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "Magic: " << PLAYER_STATS.magic << "   |\n";
        break;
    case 12:
        std::cout << '|';
        PrintCenter("Wealth: " + std::to_string(PLAYER_STATS.dun) + " dun", HUD_PANEL_SIZE - 2, true);
        std::cout << "|\n";
        break;
    case 14:
        std::cout << '|';
        PrintCenter("[i]tems", HUD_PANEL_SIZE / 2 - 1, true);
        PrintCenter("[s]kills", HUD_PANEL_SIZE / 2 - 1, true);
        std::cout << "|\n";
        break;
    case 15:
        std::cout << '|';
        PrintCenter("[m]ap ", HUD_PANEL_SIZE / 2 - 1, true);
        PrintCenter("[h]elp  ", HUD_PANEL_SIZE / 2 - 1, true);
        std::cout << "|\n";
        break;
    case 16:
        std::cout << '|';
        PrintCenter("[q]uit", HUD_PANEL_SIZE - 2, true);
        std::cout << "|\n";
        break;
    case 19:
        std::cout << '|';
        if (m_CurrentRoom->getParentWorld()->getPlayer()->touching() != nullptr)
        {
            PrintCenter(m_CurrentRoom->getParentWorld()->getPlayer()->touching()->getName(), HUD_PANEL_SIZE - 2, true);
        }
        else
        {
            for (int i = 0; i < HUD_PANEL_SIZE - 2; i++)
            {
                std::cout << ' ';
            }
        }
        std::cout << "|\n";
        break;
    case 20:
        std::cout << '|';
        if (m_CurrentRoom->getParentWorld()->GetPlayer()->touching() != nullptr)
        {
            PrintCenter(m_CurrentRoom->getParentWorld()->GetPlayer()->touching()->getDesc(), HUD_PANEL_SIZE - 2, true);
        }
        else
        {
            for (int i = 0; i < HUD_PANEL_SIZE - 2; i++)
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
    for (int i = 0; i < MAP_PANEL_SIZE; i++)
    {
        std::cout << ' ';
    }
    PrintHUDRow(0);

    // ...and the rest (getMapRow returns blank string if passed invalid row)
    for (int i = 1; i < 22; i++)
    {
        PrintCenter(GetMapRow(i - 1), MAP_PANEL_SIZE, true);
        PrintHUDRow(i);
    }

    // prompt separator
    std::cout << '+';
    for (int i = 0; i < WINDOW_WIDTH - 2; i++)
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

Worlds::Room* Screen::GetCurrentRoom()
{
    return m_CurrentRoom;
}

void Screen::SetCurrentRoom(Worlds::Room* roomPtr)
{
    m_CurrentRoom = roomPtr;
}

void Screen::Clear()
{
    std::cout << "\033[2J\033[;H";
}

} /* namespace UI */
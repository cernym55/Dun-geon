#include "screen.h"
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
#define PLAYER_STATS currentRoom->getParentWorld()->getPlayer()->getStats()
#define PLAYER_HEALTH_PC std::lround(PLAYER_STATS.health / 1.0 / PLAYER_STATS.healthMax * 100)
#define PLAYER_MANA_PC std::lround(PLAYER_STATS.mana / 1.0 / PLAYER_STATS.manaMax * 100)

namespace UI
{

Screen::Screen(Parser& parser)
    : pars(parser)
{
}

Parser& Screen::parser()
{
    return pars;
}

void Screen::printCenter(std::string str, int spaceWidth, bool secondPad)
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

void Screen::mainMenu()
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
    printCenter(splashMsg[splashNumber], WINDOW_WIDTH, false);
    std::cout << "\n\n\n";
    printCenter("Select an option:", WINDOW_WIDTH, false);
    std::cout << std::endl;
    printCenter("b - [b]egin", WINDOW_WIDTH, false);
    std::cout << std::endl;
    printCenter("h - [h]elp", WINDOW_WIDTH, false);
    std::cout << std::endl;
    printCenter("q - [q]uit", WINDOW_WIDTH, false);
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
        parser().setQuit();
        break;
    }
}

char Screen::getFieldIcon(int fieldX, int fieldY)
{
    char fieldIcon;
    if (currentRoom->getField(fieldX, fieldY)->isWall)
    {
        fieldIcon = '#'; // if wall
    }
    else if (currentRoom->getField(fieldX, fieldY)->content == nullptr)
    {
        fieldIcon = ' '; // blank space if not wall nor entity (empty)
    }
    else
    {
        fieldIcon = currentRoom->getField(fieldX, fieldY)->content->getIcon(); // else get icon from entity
    }
    return fieldIcon;
}

std::string Screen::getMapRow(int rowNumber)
{
    if (rowNumber > currentRoom->getDimY() - 1)
    {
        return "";
    }
    else
    {
        std::string mapRow = "";
        for (int i = 0; i < currentRoom->getDimX(); i++)
        {
            mapRow += getFieldIcon(i, rowNumber);
        }
        return mapRow;
    }
}

void Screen::printHUDRow(int rowNumber)
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
        std::cout << "|   World " << currentRoom->getParentWorld()->getNum();
        for (i = 0; i < 10 - std::to_string(currentRoom->getRoomNum()).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "Room " << currentRoom->getRoomNum() << "   |\n";
        break;
    case 4:
        std::cout << '|';
        printCenter(currentRoom->getParentWorld()->getPlayer()->getName(), HUD_PANEL_SIZE - 2, true);
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
        printCenter("Wealth: " + std::to_string(PLAYER_STATS.dun) + " dun", HUD_PANEL_SIZE - 2, true);
        std::cout << "|\n";
        break;
    case 14:
        std::cout << '|';
        printCenter("[i]tems", HUD_PANEL_SIZE / 2 - 1, true);
        printCenter("[s]kills", HUD_PANEL_SIZE / 2 - 1, true);
        std::cout << "|\n";
        break;
    case 15:
        std::cout << '|';
        printCenter("[m]ap ", HUD_PANEL_SIZE / 2 - 1, true);
        printCenter("[h]elp  ", HUD_PANEL_SIZE / 2 - 1, true);
        std::cout << "|\n";
        break;
    case 16:
        std::cout << '|';
        printCenter("[q]uit", HUD_PANEL_SIZE - 2, true);
        std::cout << "|\n";
        break;
    case 19:
        std::cout << '|';
        if (currentRoom->getParentWorld()->getPlayer()->touching() != nullptr)
        {
            printCenter(currentRoom->getParentWorld()->getPlayer()->touching()->getName(), HUD_PANEL_SIZE - 2, true);
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
        if (currentRoom->getParentWorld()->getPlayer()->touching() != nullptr)
        {
            printCenter(currentRoom->getParentWorld()->getPlayer()->touching()->getDesc(), HUD_PANEL_SIZE - 2, true);
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

void Screen::draw()
{
    // line 1
    clear();
    for (int i = 0; i < MAP_PANEL_SIZE; i++)
    {
        std::cout << ' ';
    }
    printHUDRow(0);

    // ...and the rest (getMapRow returns blank string if passed invalid row)
    for (int i = 1; i < 22; i++)
    {
        printCenter(getMapRow(i - 1), MAP_PANEL_SIZE, true);
        printHUDRow(i);
    }

    // prompt separator
    std::cout << '+';
    for (int i = 0; i < WINDOW_WIDTH - 2; i++)
    {
        std::cout << '-';
    }
    std::cout << "+\n";

    // message + prompt
    std::cout << "  " << parser().getMessage() << "\n  > ";
}

void Screen::drawInventory()
{
}

void Screen::drawBattle()
{
}

Mode Screen::getMode()
{
    return mode;
}

void Screen::setMode(Mode m)
{
    mode = m;
}

Worlds::Room* Screen::getCurrentRoom()
{
    return currentRoom;
}

void Screen::setCurrentRoom(Worlds::Room* roomPtr)
{
    currentRoom = roomPtr;
}

void Screen::clear()
{
    std::cout << "\033[2J\033[;H";
}

} /* namespace UI */
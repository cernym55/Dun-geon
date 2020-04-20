#include "Screen.h"
#include "Application/Application.h"
#include "Entities/EntityManager.h"
#include "Entities/Player.h"
#include "Misc/Coords.h"
#include "Misc/RNG.h"
#include "Misc/Utils.h"
#include "Parser.h"
#include "Worlds/Field.h"
#include "Worlds/Room.h"
#include "Worlds/World.h"
#include "Worlds/WorldManager.h"
#include <cmath>
#include <limits>

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 24
#define MAP_PANEL_SIZE 50
#define HUD_PANEL_SIZE 30
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
    int splashNumber = RNG::RandomInt(splashMsg.size());
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
        std::cout << "|   World " << m_WorldManager.GetCurrentWorld().GetWorldNumber();
        for (i = 0; i < 10 - std::to_string(m_WorldManager.GetCurrentRoom().GetRoomNumber()).size(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "Room " << m_WorldManager.GetCurrentRoom().GetRoomNumber() << "   |\n";
        break;
    case 4:
        std::cout << '|';
        PrintCenter(m_Player.GetName(), HUD_PANEL_SIZE - 2, true);
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
        PrintCenter("Wealth: " + std::to_string(stats.dun) + " dun", HUD_PANEL_SIZE - 2, true);
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
        if (m_EntityManager.GetApproachedEntity(m_Player) != nullptr)
        {
            PrintCenter(m_EntityManager.GetApproachedEntity(m_Player)->GetName(), HUD_PANEL_SIZE - 2, true);
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
        if (m_EntityManager.GetApproachedEntity(m_Player) != nullptr)
        {
            PrintCenter(m_EntityManager.GetApproachedEntity(m_Player)->GetDescription(), HUD_PANEL_SIZE - 2, true);
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

void Screen::Clear()
{
    std::cout << "\033[2J\033[;H";
}

} /* namespace UI */
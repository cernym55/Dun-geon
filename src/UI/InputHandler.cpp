#include "InputHandler.h"
#include "ColorPairs.h"
#include "Misc/Direction.h"
#include "Misc/Utils.h"
#include "Player/Controller.h"
#include "Screen.h"
#include "Worlds/Room.h"
#include "Worlds/World.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <ncurses.h>
#include <sstream>

namespace UI
{

InputHandler::InputHandler(Screen& screen, Player::Controller& playerController)
    : m_Screen(screen), m_PlayerController(playerController), m_ShouldQuit(false)
{
    m_ShouldQuit = false;
    m_CmdDict["wait"] = CommandType::None;
    m_CmdDict["nil"] = CommandType::None;
    m_CmdDict["go"] = CommandType::Move;
    m_CmdDict["move"] = CommandType::Move;
    m_CmdDict["walk"] = CommandType::Move;
    m_CmdDict["g"] = CommandType::Move;
    m_CmdDict["get"] = CommandType::Get;
    m_CmdDict["grab"] = CommandType::Get;
    m_CmdDict["take"] = CommandType::Get;
    m_CmdDict["loot"] = CommandType::Get;
    m_CmdDict["acquire"] = CommandType::Get;
    m_CmdDict["break"] = CommandType::Break;
    m_CmdDict["smash"] = CommandType::Break;
    m_CmdDict["destroy"] = CommandType::Break;
    m_CmdDict["battle"] = CommandType::Battle;
    m_CmdDict["fight"] = CommandType::Battle;
    m_CmdDict["duel"] = CommandType::Battle;
    m_CmdDict["challenge"] = CommandType::Battle;
    m_CmdDict["speak"] = CommandType::Talk;
    m_CmdDict["talk"] = CommandType::Talk;
    m_CmdDict["hail"] = CommandType::Talk;
    m_CmdDict["trade"] = CommandType::Trade;
    m_CmdDict["buy"] = CommandType::Trade;
    m_CmdDict["sell"] = CommandType::Trade;
    m_UICmdDict["i"] = UICommandType::Inventory;
    m_UICmdDict["inv"] = UICommandType::Inventory;
    m_UICmdDict["inventory"] = UICommandType::Inventory;
    m_UICmdDict["items"] = UICommandType::Inventory;
    m_UICmdDict["bag"] = UICommandType::Inventory;
    m_UICmdDict["backpack"] = UICommandType::Inventory;
    m_UICmdDict["s"] = UICommandType::Skills;
    m_UICmdDict["skills"] = UICommandType::Skills;
    m_UICmdDict["abilities"] = UICommandType::Skills;
    m_UICmdDict["m"] = UICommandType::Map;
    m_UICmdDict["map"] = UICommandType::Map;
    m_UICmdDict["h"] = UICommandType::Help;
    m_UICmdDict["help"] = UICommandType::Help;
    m_UICmdDict["helpscreen"] = UICommandType::Help;
    m_UICmdDict["f1"] = UICommandType::Help;
    m_UICmdDict["q"] = UICommandType::Quit;
    m_UICmdDict["quit"] = UICommandType::Quit;
    m_UICmdDict["exit"] = UICommandType::Quit;
    m_DirDict["u"] = Direction::Up();
    m_DirDict["up"] = Direction::Up();
    m_DirDict["r"] = Direction::Right();
    m_DirDict["right"] = Direction::Right();
    m_DirDict["d"] = Direction::Down();
    m_DirDict["down"] = Direction::Down();
    m_DirDict["l"] = Direction::Left();
    m_DirDict["left"] = Direction::Left();
    m_AndKeywords = { "and", "&", "then" };
    m_LastKeywords = { "a", "last", "repeat", "again" };
    makeKeyConf();
    loadKeyConf();
}

void InputHandler::ExecCommandQueue()
{
    while (!m_CommandQueue.empty())
    {
        Command cmd = m_CommandQueue.front();
        m_CommandQueue.pop();
        while (cmd.repeats > 0)
        {
            // Empty queue on unsuccessful command
            if (!ExecCommand(cmd))
            {
                while (!m_CommandQueue.empty())
                    m_CommandQueue.pop();
                break;
            }
        }
    }
}

void InputHandler::Eval(const std::string& input)
{
    std::vector<std::string> words;
    std::istringstream iss(input);
    std::string buffer;
    while (iss >> buffer)
    {
        // Interrupt if an UI command is detected
        if (m_UICmdDict.count(buffer) > 0)
        {
            std::string next;
            if (words.empty() && !(iss >> next))
            {
                ExecUICommand(m_UICmdDict.at(buffer));
            }
            else
            {
                m_Screen.PostMessage("User interface commands such as \"quit\" cannot be chained or have parameters.");
            }
            return;
        }
        words.push_back(std::move(buffer));
    }

    switch (m_Screen.GetView())
    {
    case Screen::View::InGame:
        EvalWorld(words);
        break;
    default:
        return;
    }
}

bool InputHandler::ShouldQuit() const
{
    return m_ShouldQuit;
}

void InputHandler::SetQuit()
{
    m_ShouldQuit = true;
}

void InputHandler::makeKeyConf() const
{
    if (fileExists("data/controls.conf"))
    {
        return;
    }
    else
    {
        std::ofstream file;
        file.open("data/controls.conf");
        file << "# Dun-geon command keyword configuration file\n\n"
             << "# This file is auto-generated if missing. To reset your controls to default, simply delete it.\n"
             << "# A line consists of a \"KEYWORD_CODE\" followed by space-separated keywords (aliases) for that command.\n"
             << "# Numbers, uppercase letters and special characters other than '&' are not allowed.\n"
             << "# Lines starting with '#' are ignored.\n\n"
             << "# --- WORLD MOVEMENT AND ACTIONS ---\n"
             << "# Blank \"do nothing\" command.\n"
             << "NIL wait nil\n"
             << "# Move the player character. Requires a direction argument.\n"
             << "MOVE go move walk g\n"
             << "# Pick up an item next to the player. Requires a direction argument.\n"
             << "GET get grab take loot acquire\n"
             << "# Break a destructible object next to the player. Requires a direction argument.\n"
             << "BREAK break smash destroy\n"
             << "# Fight a combat-enabled character next to the player. Requires a direction argument.\n"
             << "BATTLE battle fight duel challenge\n"
             << "# Talk to a character next to the player. Requires a direction argument.\n"
             << "TALK speak talk hail\n"
             << "# Trade with a merchant character next to the player. Requires a direction argument.\n"
             << "TRADE trade buy sell\n"
             << "# Open the player's inventory.\n"
             << "OPEN_INV i inv inventory items bag backpack\n"
             << "# Open the player's skill menu.\n"
             << "OPEN_SKILLS s skills abilities\n"
             << "# Display the world map.\n"
             << "OPEN_MAP m map\n"
             << "# Display the ingame help section.\n"
             << "OPEN_HELP h help helpscreen f1\n"
             << "# Quit the game.\n"
             << "QUIT q quit exit\n"
             << "# Keyword for direction UP.\n"
             << "UP u up\n"
             << "# Keyword for direction RIGHT.\n"
             << "RIGHT r right\n"
             << "# Keyword for direction DOWN.\n"
             << "DOWN d down\n"
             << "# Keyword for direction LEFT.\n"
             << "LEFT l left\n"
             << "# Keyword for chaining commands.\n"
             << "AND & and then\n"
             << "# Repeat the last single command.\n"
             << "LAST a last repeat again\n";
        file.close();
    }
}

void InputHandler::loadKeyConf()
{
    if (!fileExists("data/controls.conf"))
    {
        m_Screen.PostMessage("Could not read file \"controls.conf\". Controls set to default.");
        return;
    }
    else
    {
        m_CmdDict.clear();
        m_DirDict.clear();
        m_AndKeywords.clear();
        m_LastKeywords.clear();
        std::ifstream file("data/controls.conf");
        std::string line;
        std::vector<std::string> wordVec;
        std::string buffer;
        // parse file 1 line at a time (split on whitespace, stop on comment)
        while (file.good())
        {
            wordVec.clear();
            buffer.clear();
            line.clear();
            std::getline(file, line);
            line += " ";
            for (size_t i = 0; i < line.size(); i++)
            {
                if (std::isalpha(line[i]) || line[i] == '_' || line[i] == '&')
                {
                    buffer += line[i];
                }
                else
                {
                    if (!buffer.empty())
                    {
                        wordVec.push_back(buffer);
                        buffer.clear();
                    }
                    if (line[i] == '#')
                    {
                        break;
                    }
                }
            }
            if (wordVec.size() >= 2)
            {
                // Whoever is reading this, forgive me
                // TODO: correct this heresy
                if (wordVec[0] == "CommandType::NIL")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_CmdDict[wordVec[i]] = CommandType::None;
                    }
                }
                else if (wordVec[0] == "MOVE")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_CmdDict[wordVec[i]] = CommandType::Move;
                    }
                }
                else if (wordVec[0] == "GET")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_CmdDict[wordVec[i]] = CommandType::Get;
                    }
                }
                else if (wordVec[0] == "BREAK")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_CmdDict[wordVec[i]] = CommandType::Break;
                    }
                }
                else if (wordVec[0] == "BATTLE")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_CmdDict[wordVec[i]] = CommandType::Battle;
                    }
                }
                else if (wordVec[0] == "TALK")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_CmdDict[wordVec[i]] = CommandType::Talk;
                    }
                }
                else if (wordVec[0] == "TRADE")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_CmdDict[wordVec[i]] = CommandType::Trade;
                    }
                }
                else if (wordVec[0] == "OPEN_INV")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_UICmdDict[wordVec[i]] = UICommandType::Inventory;
                    }
                }
                else if (wordVec[0] == "OPEN_SKILLS")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_UICmdDict[wordVec[i]] = UICommandType::Skills;
                    }
                }
                else if (wordVec[0] == "OPEN_MAP")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_UICmdDict[wordVec[i]] = UICommandType::Map;
                    }
                }
                else if (wordVec[0] == "OPEN_HELP")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_UICmdDict[wordVec[i]] = UICommandType::Help;
                    }
                }
                else if (wordVec[0] == "QUIT")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_UICmdDict[wordVec[i]] = UICommandType::Quit;
                    }
                }
                else if (wordVec[0] == "UP")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_DirDict[wordVec[i]] = Direction::Up();
                    }
                }
                else if (wordVec[0] == "RIGHT")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_DirDict[wordVec[i]] = Direction::Right();
                    }
                }
                else if (wordVec[0] == "DOWN")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_DirDict[wordVec[i]] = Direction::Down();
                    }
                }
                else if (wordVec[0] == "LEFT")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_DirDict[wordVec[i]] = Direction::Left();
                    }
                }
                else if (wordVec[0] == "AND")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_AndKeywords.push_back(wordVec[i]);
                    }
                }
                else if (wordVec[0] == "LAST")
                {
                    for (size_t i = 1; i < wordVec.size(); i++)
                    {
                        m_LastKeywords.push_back(wordVec[i]);
                    }
                }
            }
        }
    }
}

void InputHandler::HandleNextKeyInput()
{
    static const std::string CannotMoveMessage = "Cannot move there.";
    int key = getch();
    switch (key)
    {
    case 'w':
    case KEY_UP:
        m_CommandQueue.emplace(CommandType::Move, Direction::Up(), 1);
        break;
    case 'd':
    case KEY_RIGHT:
        m_CommandQueue.emplace(CommandType::Move, Direction::Right(), 1);
        break;
    case 's':
    case KEY_DOWN:
        m_CommandQueue.emplace(CommandType::Move, Direction::Down(), 1);
        break;
    case 'a':
    case KEY_LEFT:
        m_CommandQueue.emplace(CommandType::Move, Direction::Left(), 1);
        break;
    case ' ': {
        std::string input = GetTextInputFromPrompt();
        if (!input.empty())
        {
            Eval(input);
        }
        break;
    }
    case 'm':
        ExecUICommand(UICommandType::Map);
        break;
    case 'q':
        ExecUICommand(UICommandType::Quit);
        break;
    }

    if (!m_ShouldQuit) ExecCommandQueue();
}

InputHandler::Command::Command()
    : type(CommandType::None), dir(Direction::None()), repeats(1)
{
}

InputHandler::Command::Command(InputHandler::CommandType type, Direction dir, int repeats)
    : type(type), dir(dir), repeats(repeats)
{
}

bool InputHandler::ExecCommand(Command& command)
{
    std::ostringstream messageStream;
    bool result = true;
    switch (command.type)
    {
    case CommandType::None:
        break;
    case CommandType::Move:
        if (command.dir == Direction::None())
        {
            messageStream << "No direction given.";
            result = false;
        }
        else if (!m_PlayerController.TryMovePlayer(command.dir))
        {
            messageStream << "Cannot move there.";
            result = false;
        }
        break;
    case CommandType::Get:
        // TODO: add
        break;
    case CommandType::Break:
        // TODO: add
        break;
    case CommandType::Battle:
        // TODO: add
        break;
    case CommandType::Talk:
        // TODO: add
        break;
    case CommandType::Trade:
        // TODO: add
        break;
    default:
        break;
    }

    std::string message = messageStream.str();
    if (!message.empty())
    {
        m_Screen.PostMessage(messageStream.str());
    }

    command.repeats--;
    return result;
}

void InputHandler::ExecUICommand(UICommandType type)
{
    switch (type)
    {
    case UICommandType::Map:
        m_Screen.ShowMap();
        break;
    case UICommandType::Quit:
        if (m_Screen.YesNoMessageBox("Are you sure you want to quit?"))
        {
            SetQuit();
        }
        break;
    default:
        break;
    }
}

void InputHandler::EvalWorld(std::vector<std::string>& words)
{
    static const std::string CommandNotUnderstoodMessage = "Command not understood.";
    if (words.empty())
    {
        m_Screen.PostMessage(CommandNotUnderstoodMessage);
        return;
    }

    auto nextAndKeyword = words.begin();

    do
    {
        Command cmd;
        bool lastCalled = false;
        nextAndKeyword = std::find_if(words.begin(),
                                      words.end(),
                                      [&, words, this](const std::string& candidate) {
                                          return std::find(m_AndKeywords.begin(), m_AndKeywords.end(), candidate) != m_AndKeywords.end();
                                      });
        bool understood = false;
        // Look for command keywords
        for (auto it = words.begin(); it != nextAndKeyword; it++)
        {
            if (m_CmdDict.count(*it) > 0)
            {
                cmd.type = m_CmdDict[*it];
                understood = true;
                break;
            }
            else if (std::find(m_LastKeywords.begin(), m_LastKeywords.end(), *it) != m_LastKeywords.end())
            {
                // look for LAST
                cmd = m_LastCommand;
                lastCalled = true;
                understood = true;
                break;
            }
        }
        if (!understood)
        {
            m_Screen.PostMessage(CommandNotUnderstoodMessage);
            break;
        }
        for (auto it = words.begin(); it != nextAndKeyword; it++)
        {
            // look for numbers
            int repeats = std::atoi(it->c_str());
            if (repeats > 0)
            {
                cmd.repeats = repeats;
                if (lastCalled)
                {
                    cmd.repeats *= m_LastCommand.repeats;
                }
                if (cmd.repeats > 500) cmd.repeats = 500;
            }

            // look for direction keywords
            if (m_DirDict.count(*it) > 0)
            {
                cmd.dir = m_DirDict[*it];
            }
        }

        m_CommandQueue.push(cmd);

        m_LastCommand = cmd;

        if (nextAndKeyword != words.end())
        {
            words.erase(words.begin(), nextAndKeyword + 1);
            nextAndKeyword = words.begin();
        }
    } while (nextAndKeyword != words.end());
}

std::string InputHandler::GetTextInputFromPrompt()
{
    WINDOW* inputWindow = newwin(3,
                                 Screen::ScreenWidth - 20,
                                 Screen::ScreenHeight - 3,
                                 10);
    // Draw borders and prompt
    wborder(inputWindow, 0, 0, 0, 0, 0, 0, ACS_BTEE, ACS_BTEE);
    mvwaddch(inputWindow, 0, Screen::WorldPanelWidth - 10, ACS_BTEE);
    wattron(inputWindow, A_REVERSE);
    mvwaddstr(inputWindow, 0, (Screen::ScreenWidth - 20 - 14) / 2, " Command Input ");
    wattroff(inputWindow, A_REVERSE);
    mvwaddstr(inputWindow, 1, 2, "> ");
    wrefresh(inputWindow);

    // We have to handle the line input ourselves in order to allow the ESC cancel
    curs_set(1);
    wattron(inputWindow, COLOR_PAIR(ColorPairs::YellowText));
    std::string input;
    input.resize(Screen::ScreenWidth - 25, 0);
    int ch;
    size_t pos = 0;
    while ((ch = wgetch(inputWindow)) != '\n' && ch != 27)
    {
        if (pos > 0 && (ch == KEY_BACKSPACE || ch == 127 || ch == '\b'))
        {
            pos--;
            input[pos] = 0;
            mvwaddch(inputWindow, 1, 4 + pos, ' ');
        }
        else if (pos < Screen::ScreenWidth - 26 && ch != KEY_BACKSPACE && ch != 127 && ch != '\b')
        {
            input[pos] = ch;
            waddch(inputWindow, ch);
            pos++;
        }
        wmove(inputWindow, 1, 4 + pos);
        wrefresh(inputWindow);
    }
    if (ch == 27)
    {
        input.clear();
    }
    else
    {
        input.resize(input.find_first_of('\0'));
    }

    wattroff(inputWindow, A_COLOR);
    curs_set(0);

    delwin(inputWindow);
    return input;
}

} /* namespace UI */
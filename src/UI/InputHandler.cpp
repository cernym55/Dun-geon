#include "InputHandler.h"
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
#include <sstream>

#define KEY_CONF_FILENAME "data/controls.conf"

namespace UI
{

InputHandler::InputHandler(Screen& screen, Player::Controller& playerController)
    : m_Screen(screen), m_PlayerController(playerController)
{
    quitCommand = false;
    cmdDict["wait"] = NIL;
    cmdDict["nil"] = NIL;
    cmdDict["go"] = MOVE;
    cmdDict["move"] = MOVE;
    cmdDict["walk"] = MOVE;
    cmdDict["g"] = MOVE;
    cmdDict["get"] = GET;
    cmdDict["grab"] = GET;
    cmdDict["take"] = GET;
    cmdDict["loot"] = GET;
    cmdDict["acquire"] = GET;
    cmdDict["break"] = BREAK;
    cmdDict["smash"] = BREAK;
    cmdDict["destroy"] = BREAK;
    cmdDict["battle"] = BATTLE;
    cmdDict["fight"] = BATTLE;
    cmdDict["duel"] = BATTLE;
    cmdDict["challenge"] = BATTLE;
    cmdDict["speak"] = TALK;
    cmdDict["talk"] = TALK;
    cmdDict["hail"] = TALK;
    cmdDict["trade"] = TRADE;
    cmdDict["buy"] = TRADE;
    cmdDict["sell"] = TRADE;
    cmdDict["i"] = OPEN_INV;
    cmdDict["inv"] = OPEN_INV;
    cmdDict["inventory"] = OPEN_INV;
    cmdDict["items"] = OPEN_INV;
    cmdDict["bag"] = OPEN_INV;
    cmdDict["backpack"] = OPEN_INV;
    cmdDict["s"] = OPEN_SKILLS;
    cmdDict["skills"] = OPEN_SKILLS;
    cmdDict["abilities"] = OPEN_SKILLS;
    cmdDict["m"] = OPEN_MAP;
    cmdDict["map"] = OPEN_MAP;
    cmdDict["h"] = OPEN_HELP;
    cmdDict["help"] = OPEN_HELP;
    cmdDict["helpscreen"] = OPEN_HELP;
    cmdDict["f1"] = OPEN_HELP;
    cmdDict["q"] = QUIT;
    cmdDict["quit"] = QUIT;
    cmdDict["exit"] = QUIT;
    dirDict["u"] = Direction::Up();
    dirDict["up"] = Direction::Up();
    dirDict["r"] = Direction::Right();
    dirDict["right"] = Direction::Right();
    dirDict["d"] = Direction::Down();
    dirDict["down"] = Direction::Down();
    dirDict["l"] = Direction::Left();
    dirDict["left"] = Direction::Left();
    andKeywords = { "and", "&", "then" };
    lastKeywords = { "a", "last", "repeat", "again" };
    makeKeyConf();
    loadKeyConf();
    last.type = NIL;
    last.dir = Direction::None();
    last.rep = 1;
}

void InputHandler::evalWorld()
{
    if (words.empty())
    {
        message = "Command not understood.";
        return;
    }
    Command cmd;
    cmd.type = NIL;
    cmd.dir = Direction::None();
    cmd.rep = 1;
    int andLoc = -1;
    bool lastCalled = false;
    // look for AND keyword
    for (int i = 0; i < words.size(); i++)
    {
        if (i < words.size() - 1 && std::find(andKeywords.begin(), andKeywords.end(), words[i]) != andKeywords.end())
        {
            andLoc = i;
            break;
        }
    }
    // only read up to AND if found
    int workingLength = (andLoc == -1) ? words.size() : andLoc;
    // look for command keywords
    for (int i = 0; i < workingLength; i++)
    {
        if (cmdDict.find(words[i]) != cmdDict.end())
        {
            cmd.type = cmdDict.find(words[i])->second;
            break;
        }
        else if (std::find(lastKeywords.begin(), lastKeywords.end(), words[i]) != lastKeywords.end())
        {
            // look for LAST
            cmd = last;
            lastCalled = true;
            break;
        }
        else
        {
            message = "Command not understood.";
        }
    }
    // look for numbers
    for (int i = 0; i < workingLength; i++)
    {
        if (std::atoi(words[i].c_str()) > 0)
        {
            cmd.rep = lastCalled ? last.rep * std::atoi(words[i].c_str()) : std::atoi(words[i].c_str());
            break;
        }
    }
    // look for direction keywords if command is not being repeated
    for (int i = 0; i < workingLength; i++)
    {
        if (dirDict.find(words[i]) != dirDict.end())
        {
            cmd.dir = dirDict.find(words[i])->second;
            break;
        }
    }
    // push command to queue cmd.rep times
    for (int i = 0; i < cmd.rep; i++)
    {
        cmdQueue.push_back(cmd);
    }
    last = cmd;
    if (andLoc >= 0)
    {
        words.erase(words.begin(), words.begin() + andLoc + 1);
        eval();
    }
}

std::deque<Command>& InputHandler::getCmdQueue()
{
    return cmdQueue;
}

void InputHandler::execCommand()
{
    if (cmdQueue.empty())
    {
        return;
    }
    // exec & pop the command in front of queue
    CmdType type = cmdQueue.front().type;
    Direction dir = cmdQueue.front().dir;
    int rep = cmdQueue.front().rep;
    std::ostringstream messageStream;
    switch (type)
    {
    case NIL:
        // drop through to move command if dir given
        if (dir == Direction::None())
        {
            break;
        }
    case MOVE:
        if (dir == Direction::None())
        {
            messageStream << "No direction given.";
        }
        else if (!m_PlayerController.TryMovePlayer(dir))
        {
            messageStream << "Cannot move there.";
        }
        else
        {
            messageStream
                << "Moved " << dir << " " << rep
                << ((rep == 1)
                        ? " field."
                        : " fields.");
        }
        break;
    case GET:
        // TODO: add
        break;
    case BREAK:
        // TODO: add
        break;
    case BATTLE:
        // TODO: add
        break;
    case TALK:
        // TODO: add
        break;
    case TRADE:
        // TODO: add
        break;
    case OPEN_INV:
        // TODO: add
        break;
    case OPEN_SKILLS:
        // TODO: add
        break;
    case OPEN_MAP:
        // TODO: add
        break;
    case OPEN_HELP:
        // TODO: add
        break;
    case QUIT:
        setQuit();
        break;
    }
    cmdQueue.pop_front();
    message = messageStream.str();
}

Direction InputHandler::findDir(std::string input)
{
    if (dirDict.find(input) != dirDict.end())
    {
        return dirDict.find(input)->second;
    }
    else
    {
        return Direction::None();
    }
}

CmdType InputHandler::findCmdType(std::string input)
{
    if (cmdDict.find(input) != cmdDict.end())
    {
        return cmdDict.find(input)->second;
    }
    else
    {
        return NIL;
    }
}

void InputHandler::readInput()
{
    std::cin.clear();
    std::getline(std::cin, input);
}

void InputHandler::parse()
{
    words.clear();
    std::string wordBuffer = "";
    // append a space for parsing purposes
    input += " ";
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
        {
            if (std::isalnum(input[i]) || input[i] == '&')
            {
                wordBuffer += std::tolower(input[i]);
            }
        }
        else
        {
            if (!wordBuffer.empty())
            {
                words.push_back(wordBuffer);
                wordBuffer = "";
            }
        }
    }
}

void InputHandler::eval()
{
    switch (m_Screen.GetView())
    {
    case Screen::View::World:
        evalWorld();
        break;
    case Screen::View::Test:
        message = "";
        for (int i = 0; i < words.size(); i++)
        {
            message += words[i];
        }
        break;
    }
}

bool InputHandler::quit()
{
    return quitCommand;
}

void InputHandler::setQuit()
{
    quitCommand = true;
}

std::string InputHandler::getMessage()
{
    return message;
}

void InputHandler::setMessage(std::string value)
{
    message = value;
}

void InputHandler::makeKeyConf()
{
    if (fileExists(KEY_CONF_FILENAME))
    {
        return;
    }
    else
    {
        std::ofstream file;
        file.open(KEY_CONF_FILENAME);
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
    if (!fileExists(KEY_CONF_FILENAME))
    {
        message = "Could not read file \"controls.conf\". Controls set to default.";
        return;
    }
    else
    {
        cmdDict.clear();
        dirDict.clear();
        andKeywords.clear();
        lastKeywords.clear();
        std::ifstream file(KEY_CONF_FILENAME);
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
            for (int i = 0; i < line.size(); i++)
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
                if (wordVec[0] == "NIL")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = NIL;
                    }
                }
                else if (wordVec[0] == "MOVE")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = MOVE;
                    }
                }
                else if (wordVec[0] == "GET")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = GET;
                    }
                }
                else if (wordVec[0] == "BREAK")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = BREAK;
                    }
                }
                else if (wordVec[0] == "BATTLE")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = BATTLE;
                    }
                }
                else if (wordVec[0] == "TALK")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = TALK;
                    }
                }
                else if (wordVec[0] == "TRADE")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = TRADE;
                    }
                }
                else if (wordVec[0] == "OPEN_INV")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = OPEN_INV;
                    }
                }
                else if (wordVec[0] == "OPEN_SKILLS")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = OPEN_SKILLS;
                    }
                }
                else if (wordVec[0] == "OPEN_MAP")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = OPEN_MAP;
                    }
                }
                else if (wordVec[0] == "OPEN_HELP")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = OPEN_HELP;
                    }
                }
                else if (wordVec[0] == "QUIT")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        cmdDict[wordVec[i]] = QUIT;
                    }
                }
                else if (wordVec[0] == "UP")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        dirDict[wordVec[i]] = Direction::Up();
                    }
                }
                else if (wordVec[0] == "RIGHT")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        dirDict[wordVec[i]] = Direction::Right();
                    }
                }
                else if (wordVec[0] == "DOWN")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        dirDict[wordVec[i]] = Direction::Down();
                    }
                }
                else if (wordVec[0] == "LEFT")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        dirDict[wordVec[i]] = Direction::Left();
                    }
                }
                else if (wordVec[0] == "AND")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        andKeywords.push_back(wordVec[i]);
                    }
                }
                else if (wordVec[0] == "LAST")
                {
                    for (int i = 1; i < wordVec.size(); i++)
                    {
                        lastKeywords.push_back(wordVec[i]);
                    }
                }
            }
        }
    }
}

} /* namespace UI */
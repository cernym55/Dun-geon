#pragma once

#include "entities/character.h"
#include <deque>
#include <iostream>
#include <map>
#include <vector>

namespace UI
{

enum CmdType
{
    NIL,
    MOVE,
    GET,
    BREAK,
    BATTLE,
    TALK,
    TRADE,
    OPEN_INV,
    OPEN_SKILLS,
    OPEN_MAP,
    OPEN_HELP,
    QUIT
};

struct Command
{
    CmdType type;
    Entities::Direction dir;
    int rep;
};

class Screen;

class Parser
{
private:
    Screen& m_Screen;
    std::string input;
    std::vector<std::string> words;
    std::string message;
    Command last;
    std::deque<Command> cmdQueue;
    std::map<std::string, Entities::Direction> dirDict;
    std::map<std::string, CmdType> cmdDict;
    std::vector<std::string> andKeywords;
    std::vector<std::string> lastKeywords;
    bool quitCommand;
    void evalWorld();

public:
    Parser(Screen& screen);
    Parser(const Parser& r) = delete;
    ~Parser() = default;
    std::deque<Command>& getCmdQueue();
    void execCommand();
    Entities::Direction findDir(std::string input);
    CmdType findCmdType(std::string input);
    void readInput();
    void parse();
    void eval();
    bool quit();
    void setQuit();
    std::string getMessage();
    void setMessage(std::string value);
    void makeKeyConf();
    void loadKeyConf();
};

} /* namespace UI */
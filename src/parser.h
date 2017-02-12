/* *
 *
 * File:  parser.h
 * Brief: Text command parser
 * 
 * Copyright (C) 2016-2017 Matyas Cerny <cerny-matyas(at)email.cz>
 *
 *
 * This file is part of Dun-geon.
 *
 * Dun-geon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dun-geon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Dun-geon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

#ifndef _PARSER_H
#define _PARSER_H

#include "character.h"
#include <iostream>
#include <vector>
#include <deque>
#include <map>

enum CmdType {
	NIL, MOVE, GET, BREAK, BATTLE, TALK, TRADE, OPEN_INV, OPEN_SKILLS, OPEN_MAP, OPEN_HELP, QUIT
};

struct Command {
	CmdType type;
	Direction dir;
	int rep;
};

class Screen;

class Parser {
private:
	Screen *screen;
	std::string input;
	std::vector<std::string> words;
	std::string message;
	Command last;
	std::deque<Command> cmdQueue;
	std::map<std::string, Direction> dirDict;
	std::map<std::string, CmdType> cmdDict;
	std::vector<std::string> andKeywords;
	std::vector<std::string> lastKeywords;
	bool quitCommand;
	void evalWorld();
public:
	Parser();
	~Parser();
	Screen *getScreen();
	void setScreen(Screen *value);
	std::deque<Command> &getCmdQueue();
	void execCommand();
	Direction findDir(std::string input);
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

#endif

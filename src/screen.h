/* *
 *
 * File:  screen.h
 * Brief: Graphical output/input interface
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

#ifndef _SCREEN_H
#define _SCREEN_H

#include "parser.h"
#include <iostream>

enum Mode {
	WORLD, TEST, INVENTORY//, BATTLE, SKILLS, INVENTORY, MAP
};

class Room;

class Screen {
	private:
		Parser pars;
		Room *currentRoom;
		Mode mode;
		std::string getMapRow(int rowNumber);
		void printHUDRow(int rowNumber);
		char getFieldIcon(int fieldX, int fieldY);
	public:
		Parser &parser();
		void printCenter(std::string str, int spaceWidth, bool secondPad);
		void mainMenu();
		void draw();
		void prompt();
		void clear();
		void drawInventory();
		void drawBattle();
		Mode getMode();
		void setMode(Mode m);
		Room *getCurrentRoom();
		void setCurrentRoom(Room *roomPtr);
};

#endif

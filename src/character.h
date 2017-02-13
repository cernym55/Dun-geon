/* *
 *
 * File:  character.h
 * Brief: Fightable characters including player
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

#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <vector>
#include "entity.h"

struct Field;
class Room;

enum Direction { up, right, down, left, nil = -1 };

struct Stats {
	int level, XP, XPToNextLevel, dun, health, healthMax, mana, manaMax, vigor, valor, haste, magic;
	//std::vector<Skill> skills; //TODO: add this when skills are done
	//std::vector<Item> inventory; //TODO: add this when items are done
};

class Character: public Entity {
	protected:
		Stats stats;
		Room *currentRoom;
		Direction lastMove;
	public:
    virtual ~Character() {};
		void move(Direction direction);
		std::vector<Field *> collision();
		Field *collision(Direction direction);
		Entity *touching();
		Stats &getStats();
		Room *getCurrentRoom();
		void setCurrentRoom(Room *roomPtr);
		Direction getLastMove();
		void setLastMove(Direction direction);
};

#endif

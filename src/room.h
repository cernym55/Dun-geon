/* *
 *
 * File:  room.h
 * Brief: Room object that is a part of the game's map
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

#ifndef _ROOM_H
#define _ROOM_H

#include "character.h"
#include <vector>

class Entity;
class World;

enum Layout {
	randLayout = -1,
	box,
	NUM_LAYOUTS = 1
};

struct Field {
	int posX, posY;
	bool isWall;
	Entity *content;
  Entity *background;
};

class Room {
	private:
		int dimX, dimY;
		int posX, posY;
		World *world;
		int roomNum;
		Field *entranceUp, *entranceLeft, *entranceRight, *entranceDown;
		Room *roomUp, *roomLeft, *roomRight, *roomDown;
		std::vector<std::vector<Field>> fields;
		std::vector<Entity *> entities;
		bool genStatus;
	public:
		Room(World *worldPtr);
		~Room();
		void loadNeighbors();
		World *getParentWorld();
		int getPosX();
		int getPosY();
		int getRoomNum();
		void setRoomNum(int value);
		void setPosX(int value);
		void setPosY(int value);
		int getDimX();
		int getDimY();
		void setDimX(int value);
		void setDimY(int value);
		Field *getEntrance(Direction dir);
		Room *getNeighbor(Direction dir);
		Field *getField(int x, int y);
		Entity *getEntity(int number);
		void addEntity(Entity *ent);
		void deleteEntity(int index);
		void updateEntityPos();
		void generate(Layout layout, bool forceUp, bool forceRight, bool forceDown, bool forceLeft);
		bool generated();
};

#endif

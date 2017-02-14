/* *
 *
 * File:  world.cpp
 * Brief: World class that represents the game's world map
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

#include "world.h"
#include "room.h"

World::World(int num, Player *pl) {
	playerPtr = pl;
	number = num;
	nextRoomNum = 1;
	rooms = std::vector<std::vector<Room *>>(WORLD_SIZE, std::vector<Room *>(WORLD_SIZE, nullptr));
	for (int i = 0; i < rooms.size(); i++) {
		for (int j = 0; j < rooms[i].size(); j++) {
			rooms[i][j] = new Room(this);
			rooms[i][j]->setPosX(i);
			rooms[i][j]->setPosY(j);
		}
	}
	// must be done after all rooms are constructed
	for (int i = 0; i < rooms.size(); i++) {
		for (int j = 0; j < rooms[i].size(); j++) {
			rooms[i][j]->loadNeighbors();
		}
	}
}

World::~World() {
	for (int i = 0; i < rooms.size(); i++) {
		for (int j = 0; j < rooms[i].size(); j++) {
			delete rooms[i][j];
		}
	}
}

int World::getNum() {
	return number;
}

Room *World::getRoom(int x, int y) {
    return x < 0 || x >= WORLD_SIZE || y < 0 || y >= WORLD_SIZE ? nullptr : rooms[x][y];
}

std::vector<std::vector<Room *>> &World::getRooms() {
	return rooms;
}

int World::getNextRoomNum() {
	return nextRoomNum;
}

void World::setNextRoomNum(int value) {
	nextRoomNum = value;
}

Player *World::getPlayer() {
	return playerPtr;
}

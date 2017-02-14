/* *
 *
 * File:  world.h
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

#ifndef _WORLD_H
#define _WORLD_H

#include <vector>

#define WORLD_SIZE 15

class Room;
class Player;

class World {
private:
    int number;
    int nextRoomNum;
    std::vector<std::vector<Room *>> rooms;
    Player *playerPtr;
public:
    World(int num, Player *pl);
    ~World();
    int getNum();
    Room *getRoom(int x, int y);
    std::vector<std::vector<Room *>> &getRooms();
    int getNextRoomNum();
    void setNextRoomNum(int value);
    Player *getPlayer();
};

#endif

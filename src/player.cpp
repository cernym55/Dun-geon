/* *
 *
 * File:  player.cpp
 * Brief: Player character/entity
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

#include "player.h"
#include "room.h"
#include "world.h"

// TODO: separate room transitions into another function
// TODO: do not move the player, handle that elsewhere using the return bool
bool Player::attemptMove(Direction dir) {
	Field *colField = collision(dir);
	if (dir != nil && colField != nullptr && !colField->isWall) {
		if (colField->content == nullptr || !colField->content->isBlocking()) {
			move(dir);
			return true;
		}
	} else if (dir != nil && colField == nullptr) {
		Direction nextEntranceDir = (Direction)((dir + 2) % 4); // opposite direction
		currentRoom = currentRoom->getNeighbor(dir);
		if (!currentRoom->generated()) {
			switch (nextEntranceDir) {
				case up:
					currentRoom->generate(randLayout, true);
					break;
				case right:
					currentRoom->generate(randLayout, false, true);
					break;
				case down:
					currentRoom->generate(randLayout, false, false, true);
					break;
				case left:
					currentRoom->generate(randLayout, false, false, false, true);
					break;
				default:
					break;
			}
		}
		posX = currentRoom->getEntrance(nextEntranceDir)->posX;
		posY = currentRoom->getEntrance(nextEntranceDir)->posY;
		return true;
	}
	return false;
}

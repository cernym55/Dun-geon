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

bool Player::attemptMove(Direction dir) {
	Field *colField = collision(dir);
	if (dir != nil && colField != nullptr && !colField->isWall) {
		if (colField->content == nullptr || !colField->content->isBlocking()) {
			move(dir);
			return true;
		}
	} else if (dir != nil && colField == nullptr) {
		Direction nextEntranceDir;
		switch (dir) {
			case up:
				nextEntranceDir = down;
				break;
			case right:
				nextEntranceDir = left;
				break;
			case down:
				nextEntranceDir = up;
				break;
			case left:
				nextEntranceDir = right;
				break;
			default:
				nextEntranceDir = nil;
				break;
		}
		currentRoom = currentRoom->getNeighbor(dir);
		if (!currentRoom->generated()) {
			switch (nextEntranceDir) {
				case up:
					currentRoom->generate(randLayout, true, false, false, false);
					break;
				case right:
					currentRoom->generate(randLayout, false, true, false, false);
					break;
				case down:
					currentRoom->generate(randLayout, false, false, true, false);
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

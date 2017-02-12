/* *
 *
 * File:  character.cpp
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

#include "character.h"
#include "room.h"

void Character::move(Direction direction) {
	switch (direction) {
		case up:
			posY -= 1;
			break;
		case right:
			posX += 1;
			break;
		case down:
			posY += 1;
			break;
		case left:
			posX -= 1;
			break;
		case nil:
			break;
	}
	lastMove = direction;
}

// Returns pointers to the 4 fields adjacent to the character
std::vector<Field *> Character::collision() {
	std::vector<Field *> colBox = {
		currentRoom->getField(posX, posY - 1),
		currentRoom->getField(posX + 1, posY),
		currentRoom->getField(posX, posY + 1),
		currentRoom->getField(posX - 1, posY)
	};
	return colBox;
}

// Returns pointer to a specific field adjacent to the character
Field *Character::collision(Direction direction) {
	if (direction == nil) {
		return currentRoom->getField(posX, posY);
	} else {
		return collision()[direction];
	}
}

// Returns whatever object the character has just walked up to (or null)
Entity *Character::touching() {
	if (lastMove >= 0 && collision(lastMove) != nullptr) {
		return collision(lastMove)->content;
	} else {
		return nullptr;
	}
}

Stats &Character::getStats() {
	return stats;
}

Room *Character::getCurrentRoom() {
	return currentRoom;
}

void Character::setCurrentRoom(Room *roomPtr) {
	currentRoom = roomPtr;
}

Direction Character::getLastMove() {
	return lastMove;
}

void Character::setLastMove(Direction direction) {
	lastMove = direction;
}

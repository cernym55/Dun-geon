/* *
 *
 * File:  entity.cpp
 * Brief: Objects that can be interacted with
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

#include "entity.h"

int Entity::getPosX() {
	return posX;
}

void Entity::setPosX(int value) {
	posX = value;
}

int Entity::getPosY() {
	return posY;
}

void Entity::setPosY(int value) {
	posY = value;
}

bool Entity::isBlocking() {
	return blocking;
}

void Entity::setBlocking(bool value) {
	blocking = value;
}

char Entity::getIcon() {
	return icon;
}

void Entity::setIcon(char value) {
	icon = value;
}

std::string Entity::getName() {
	return name;
}

void Entity::setName(std::string value) {
	name = value;
}

std::string Entity::getDesc() {
	return desc;
}

void Entity::setDesc(std::string value) {
	desc = value;
}

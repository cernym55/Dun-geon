/* *
 *
 * File:  entity.h
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

#ifndef _ENTITY_H
#define _ENTITY_H

#include <iostream>

class Entity {
	protected:
		int posX, posY;
		bool blocking;
		char icon;
		std::string name;
		std::string desc;
	public:
    virtual ~Entity() {};
		int getPosX();
		void setPosX(int value);
		int getPosY();
		void setPosY(int value);
		bool isBlocking();
		void setBlocking(bool value);
		char getIcon();
		void setIcon(char value);
		std::string getName();
		void setName(std::string value);
		std::string getDesc();
		void setDesc(std::string value);
};

#endif

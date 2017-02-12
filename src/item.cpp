/* *
 *
 * File:  item.cpp
 * Brief: Inventory items
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

#include "item.h"

std::string Item::getName() {
	return name;
}

void Item::setName(std::string value) {
	name = value;
}

std::string Item::getDesc() {
	return desc;
}

void Item::setDesc(std::string value) {
	desc = value;
}

ItemType Item::getType() {
	return itemType;
}

void Item::setType(ItemType value) {
	itemType = value;
}

bool Item::isSellable() {
	return sellable;
}

void Item::setSellable(bool value) {
	sellable = value;
}

bool Item::isDroppable() {
	return droppable;
}

void Item::setDroppable(bool value) {
	droppable = value;
}

int Item::getPrice() {
	return price;
}

void Item::setPrice(int value) {
	price = value;
}

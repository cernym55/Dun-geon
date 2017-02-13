/* *
 *
 * File:  inventory.cpp
 * Brief: Stores items carried by the player
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

#include "inventory.h"
#include "item.h"

std::vector<Item *> &Inventory::getItems() {
    return items;
}

std::size_t Inventory::getSize() {
    return items.size();
}

Item *Inventory::getItem(int index) {
    return index < 0 || index >= items.size() ? nullptr : items[index];
}

void Inventory::addItem(Item *item) {
    items.push_back(item);
}

void Inventory::removeItem(int index) {
    if (index >= 0 && index < items.size()) {
        items.erase(items.begin() + index);
    }
}

void Inventory::deleteItem(int index) {
    if (index >= 0 && index < items.size()) {
        delete items[index];
        items.erase(items.begin() + index);
    }
}

/* *
 *
 * File:  map_item_entity.cpp
 * Brief: Entity that contains an item
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

#include "map_item_entity.h"
#include "item.h"

MapItemEntity::MapItemEntity() {
    content = nullptr;
    blocking = false;
    icon = '*';
}

MapItemEntity::MapItemEntity(Item *item) {
    content = item;
    blocking = false;
    icon = '*';
}

Item *MapItemEntity::getContent() {
    return content;
}

void MapItemEntity::setContent(Item *value) {
    content = value;
}

// Remember to call this every time items are destroyed and not picked up
void MapItemEntity::deleteContent() {
    if (content != nullptr) {
        delete content;
        content = nullptr;
    }
}

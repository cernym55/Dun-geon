/* *
 *
 * File:  item.h
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

#ifndef _ITEM_H
#define _ITEM_H

#include <iostream>

enum ItemType {
    trash, consumable, key
};

class Item {
protected:
    std::string name;
    std::string desc;
    std::string prefix;
    std::string suffix;
    ItemType itemType;
    bool sellable;
    bool droppable;
    int price;
public:
    std::string getName();
    void setName(std::string value);
    std::string getDesc();
    void setDesc(std::string value);
    std::string getPrefix();
    void setPrefix(std::string value);
    std::string getSuffix();
    void setSuffix(std::string value);
    ItemType getType();
    void setType(ItemType value);
    bool isSellable();
    void setSellable(bool value);
    bool isDroppable();
    void setDroppable(bool value);
    int getPrice();
    void setPrice(int value);
};

#endif

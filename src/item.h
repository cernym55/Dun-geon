#pragma once

#include <iostream>

enum ItemType
{
    trash,
    consumable,
    key
};

class Item
{
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
    virtual ~Item(){};
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

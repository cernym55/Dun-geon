#pragma once

#include <iostream>

namespace Items
{

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
    Item();
    Item(const Item&) = delete;
    Item(Item&&) = delete;
    Item& operator=(const Item&) = delete;
    Item& operator=(Item&&) = delete;
    virtual ~Item() = default;
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

} /* namespace Items */

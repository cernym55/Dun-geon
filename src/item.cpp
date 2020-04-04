#include "item.h"

std::string Item::getName()
{
    return name;
}

void Item::setName(std::string value)
{
    name = value;
}

std::string Item::getDesc()
{
    return desc;
}

void Item::setDesc(std::string value)
{
    desc = value;
}

ItemType Item::getType()
{
    return itemType;
}

void Item::setType(ItemType value)
{
    itemType = value;
}

bool Item::isSellable()
{
    return sellable;
}

void Item::setSellable(bool value)
{
    sellable = value;
}

bool Item::isDroppable()
{
    return droppable;
}

void Item::setDroppable(bool value)
{
    droppable = value;
}

int Item::getPrice()
{
    return price;
}

void Item::setPrice(int value)
{
    price = value;
}

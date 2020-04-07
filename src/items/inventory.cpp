#include "inventory.h"
#include "item.h"

std::vector<Item*>& Inventory::getItems()
{
    return items;
}

std::size_t Inventory::getSize()
{
    return items.size();
}

Item* Inventory::getItem(int index)
{
    return index < 0 || index >= items.size() ? nullptr : items[index];
}

void Inventory::addItem(Item* item)
{
    items.push_back(item);
}

void Inventory::removeItem(int index)
{
    if (index >= 0 && index < items.size())
    {
        items.erase(items.begin() + index);
    }
}

void Inventory::deleteItem(int index)
{
    if (index >= 0 && index < items.size())
    {
        delete items[index];
        items.erase(items.begin() + index);
    }
}

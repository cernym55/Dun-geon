#pragma once

#include <vector>

class Item;

class Inventory
{
private:
    std::vector<Item*> items;
    std::vector<Item*>& getItems();

public:
    std::size_t getSize();
    Item* getItem(int index);
    void addItem(Item* item);
    void removeItem(int index);
    void deleteItem(int index);
    void sortItemsByName();
    void sortItemsByType();
    void sortItemsByCount();
};

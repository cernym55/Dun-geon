#pragma once

#include "entity.h"

class Item;

class MapItemEntity : public Entity
{
private:
    Item* content;

public:
    MapItemEntity();
    MapItemEntity(Item* item);
    Item* getContent();
    void setContent(Item* value);
    void deleteContent();
};

#pragma once

#include "entity.h"
#include "items/item.h"

namespace Entities
{

class MapItemEntity : public Entity
{
private:
    Items::Item* content;

public:
    MapItemEntity();
    MapItemEntity(Items::Item* item);
    Items::Item* getContent();
    void setContent(Items::Item* value);
    void deleteContent();
};

} /* namespace Entities */
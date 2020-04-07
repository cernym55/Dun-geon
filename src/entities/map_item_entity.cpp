#include "map_item_entity.h"
#include "items/item.h"

namespace Entities
{

MapItemEntity::MapItemEntity()
{
    content = nullptr;
    blocking = false;
    icon = '*';
}

MapItemEntity::MapItemEntity(Items::Item* item)
{
    content = item;
    blocking = false;
    icon = '*';
}

Items::Item* MapItemEntity::getContent()
{
    return content;
}

void MapItemEntity::setContent(Items::Item* value)
{
    content = value;
}

// Remember to call this every time items are destroyed and not picked up
void MapItemEntity::deleteContent()
{
    if (content != nullptr)
    {
        delete content;
        content = nullptr;
    }
}

} /* namespace Entities */
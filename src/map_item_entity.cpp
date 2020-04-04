#include "map_item_entity.h"
#include "item.h"

MapItemEntity::MapItemEntity()
{
    content = nullptr;
    blocking = false;
    icon = '*';
}

MapItemEntity::MapItemEntity(Item* item)
{
    content = item;
    blocking = false;
    icon = '*';
}

Item* MapItemEntity::getContent()
{
    return content;
}

void MapItemEntity::setContent(Item* value)
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

#include "entity.h"

namespace Entities
{

int Entity::getPosX()
{
    return posX;
}

void Entity::setPosX(int value)
{
    posX = value;
}

int Entity::getPosY()
{
    return posY;
}

void Entity::setPosY(int value)
{
    posY = value;
}

bool Entity::isBlocking()
{
    return blocking;
}

void Entity::setBlocking(bool value)
{
    blocking = value;
}

char Entity::getIcon()
{
    return icon;
}

void Entity::setIcon(char value)
{
    icon = value;
}

std::string Entity::getName()
{
    return name;
}

void Entity::setName(std::string value)
{
    name = value;
}

std::string Entity::getDesc()
{
    return desc;
}

void Entity::setDesc(std::string value)
{
    desc = value;
}

} /* namespace Entities */

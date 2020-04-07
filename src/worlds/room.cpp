#include "room.h"
#include "entities/player.h"
#include "misc/utils.h"
#include "world.h"

#define MAX_WIDTH 40
#define MAX_HEIGHT 20
#define MAX_WIDTH_DIFF 15
#define MAX_HEIGHT_DIFF 5

namespace Worlds
{

Room::Room(World* worldPtr)
{
    world = worldPtr;
    entranceUp = nullptr;
    entranceLeft = nullptr;
    entranceRight = nullptr;
    entranceDown = nullptr;
    genStatus = false;
}

Room::~Room()
{
    for (int i = 0; i < entities.size(); i++)
    {
        delete entities[i];
    }
}

void Room::loadNeighbors()
{
    roomUp = world->getRoom(posX, posY - 1);
    roomLeft = world->getRoom(posX - 1, posY);
    roomRight = world->getRoom(posX + 1, posY);
    roomDown = world->getRoom(posX, posY + 1);
}

World* Room::getParentWorld()
{
    return world;
}

int Room::getPosX()
{
    return posX;
}

int Room::getPosY()
{
    return posY;
}

int Room::getRoomNum()
{
    return roomNum;
}

void Room::setRoomNum(int value)
{
    roomNum = value;
}

void Room::setPosX(int value)
{
    posX = value;
}

void Room::setPosY(int value)
{
    posY = value;
}

int Room::getDimX()
{
    return dimX;
}

int Room::getDimY()
{
    return dimY;
}

void Room::setDimX(int value)
{
    dimX = value;
}

void Room::setDimY(int value)
{
    dimY = value;
}

Field* Room::getEntrance(Entities::Direction dir)
{
    switch (dir)
    {
    case Entities::up:
        return entranceUp;
        break;
    case Entities::right:
        return entranceRight;
        break;
    case Entities::down:
        return entranceDown;
        break;
    case Entities::left:
        return entranceLeft;
        break;
    default:
        return nullptr;
        break;
    }
}

Room* Room::getNeighbor(Entities::Direction dir)
{
    switch (dir)
    {
    case Entities::up:
        return roomUp;
        break;
    case Entities::right:
        return roomRight;
        break;
    case Entities::down:
        return roomDown;
        break;
    case Entities::left:
        return roomLeft;
        break;
    default:
        return nullptr;
        break;
    }
}

Field* Room::getField(int x, int y)
{
    if (x > dimX - 1 || x < 0 || y > dimY - 1 || y < 0)
    {
        return nullptr;
    }
    else
    {
        return &fields[x][y];
    }
}

Entities::Entity* Room::getEntity(int number)
{
    return entities[number];
}

void Room::addEntity(Entities::Entity* ent)
{
    entities.push_back(ent);
}

void Room::deleteEntity(int index)
{
    if (index < entities.size() && index >= 0 && entities[index] != nullptr)
    {
        delete entities[index];
    }
    entities.erase(entities.begin() + index);
}

void Room::updateEntityPos()
{
    // first, wipe all fields of their contained entities
    for (int i = 0; i < dimX; i++)
    {
        for (int j = 0; j < dimY; j++)
        {
            fields[i][j].content = nullptr;
        }
    }
    // then re-add them to their new location
    for (int i = 0; i < entities.size(); i++)
    {
        getField(entities[i]->getPosX(), entities[i]->getPosY())->content = entities[i];
    }
    // add the player too if they are present
    if (this == world->getPlayer()->getCurrentRoom())
    {
        getField(world->getPlayer()->getPosX(), world->getPlayer()->getPosY())->content = world->getPlayer();
    }
}

void Room::generate(Layout layout, bool forceUp, bool forceRight, bool forceDown, bool forceLeft)
{
    // generate random dimensions (within constraints)
    dimX = RNG(0, 120) % MAX_WIDTH_DIFF + MAX_WIDTH - MAX_WIDTH_DIFF;
    dimX += dimX % 2;
    dimY = RNG(0, 120) % MAX_HEIGHT_DIFF + MAX_HEIGHT - MAX_HEIGHT_DIFF;
    dimY += dimY % 2;
    Field blankField;
    blankField.content = nullptr;
    blankField.background = nullptr;
    blankField.isWall = true;

    // fill map with wall fields
    std::vector<Field> fieldsY;
    for (int i = 0; i < dimX; i++)
    {
        fields.push_back(fieldsY);
        blankField.posX = i;
        for (int j = 0; j < dimY; j++)
        {
            blankField.posY = j;
            fields[i].push_back(blankField);
        }
    }

    // check if forced entrances are needed/allowed
    bool allowUp = true, allowRight = true, allowDown = true, allowLeft = true;
    // TODO: check if neighbor rooms aren't null (posX/posY check doesn't seem to be working)
    if (getNeighbor(Entities::up) == nullptr)
    {
        allowUp = false;
        forceUp = false;
    }
    else if (getNeighbor(Entities::up)->generated())
    {
        getNeighbor(Entities::up)->getEntrance(Entities::down) == nullptr ? allowUp = false : forceUp = true;
    }
    if (getNeighbor(Entities::right) == nullptr)
    {
        allowRight = false;
        forceRight = false;
    }
    else if (getNeighbor(Entities::right)->generated())
    {
        getNeighbor(Entities::right)->getEntrance(Entities::left) == nullptr ? allowRight = false : forceRight = true;
    }
    if (getNeighbor(Entities::down) == nullptr)
    {
        allowDown = false;
        forceDown = false;
    }
    else if (getNeighbor(Entities::down)->generated())
    {
        getNeighbor(Entities::down)->getEntrance(Entities::up) == nullptr ? allowDown = false : forceDown = true;
    }
    if (getNeighbor(Entities::left) == nullptr)
    {
        allowLeft = false;
        forceLeft = false;
    }
    else if (getNeighbor(Entities::left)->generated())
    {
        getNeighbor(Entities::left)->getEntrance(Entities::right) == nullptr ? allowLeft = false : forceLeft = true;
    }

    // generate layout
    int layoutNum;
    if (layout == randLayout)
    {
        //layoutNum = RNG(0, NUM_LAYOUTS); TODO: uncomment after more layouts are added (don't ask why)
        layoutNum = 0;
    }
    else
    {
        layoutNum = layout;
    }
    switch (layoutNum)
    {
    case box:
        for (int i = 1; i < dimX - 1; i++)
        {
            for (int j = 1; j < dimY - 1; j++)
            {
                fields[i][j].isWall = false;
            }
        }
        // generate entrances (check if room is not on edge of map and if neighbor rooms can be connected)
        if (allowUp && (forceUp || RNG(0, 1) == 0))
        {
            entranceUp = getField(RNG(3, dimX - 4), 0);
            entranceUp->isWall = false;
            getField(entranceUp->posX - 1, 0)->isWall = false;
            getField(entranceUp->posX + 1, 0)->isWall = false;
        }
        if (allowDown && (forceDown || RNG(0, 1) == 0))
        {
            entranceDown = getField(RNG(3, dimX - 4), dimY - 1);
            entranceDown->isWall = false;
            getField(entranceDown->posX - 1, dimY - 1)->isWall = false;
            getField(entranceDown->posX + 1, dimY - 1)->isWall = false;
        }
        if (allowLeft && (forceLeft || RNG(0, 1) == 0))
        {
            entranceLeft = getField(0, RNG(3, dimY - 4));
            entranceLeft->isWall = false;
            getField(0, entranceLeft->posY - 1)->isWall = false;
            getField(0, entranceLeft->posY + 1)->isWall = false;
        }
        if (allowRight && (forceRight || RNG(0, 1) == 0))
        {
            entranceRight = getField(dimX - 1, RNG(3, dimY - 4));
            entranceRight->isWall = false;
            getField(dimX - 1, entranceRight->posY - 1)->isWall = false;
            getField(dimX - 1, entranceRight->posY + 1)->isWall = false;
        }
        break;
    }
    roomNum = world->getNextRoomNum();
    world->setNextRoomNum(roomNum + 1);
    genStatus = true;
}

bool Room::generated()
{
    return genStatus;
}

} /* namespace Worlds */
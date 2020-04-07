#pragma once

#include "entities/character.h"
#include "entities/entity.h"
#include <vector>

namespace Worlds
{

class World; //TODO: remove forward declaration

enum Layout
{
    randLayout = -1,
    box,
    NUM_LAYOUTS = 1
};

struct Field
{
    int posX, posY;
    bool isWall;
    Entities::Entity* content;
    Entities::Entity* background;
};

class Room
{
private:
    int dimX, dimY;
    int posX, posY;
    World* world;
    int roomNum;
    Field *entranceUp, *entranceLeft, *entranceRight, *entranceDown;
    Room *roomUp, *roomLeft, *roomRight, *roomDown;
    std::vector<std::vector<Field>> fields;
    std::vector<Entities::Entity*> entities;
    bool genStatus;

public:
    Room(World* worldPtr);
    ~Room();
    void loadNeighbors();
    World* getParentWorld();
    int getPosX();
    int getPosY();
    int getRoomNum();
    void setRoomNum(int value);
    void setPosX(int value);
    void setPosY(int value);
    int getDimX();
    int getDimY();
    void setDimX(int value);
    void setDimY(int value);
    Field* getEntrance(Entities::Direction dir);
    Room* getNeighbor(Entities::Direction dir);
    Field* getField(int x, int y);
    Entities::Entity* getEntity(int number);
    void addEntity(Entities::Entity* ent);
    void deleteEntity(int index);
    void updateEntityPos();
    void generate(Layout layout, bool forceUp = false, bool forceRight = false,
                  bool forceDown = false, bool forceLeft = false);
    bool generated();
};

} /* namespace Worlds */
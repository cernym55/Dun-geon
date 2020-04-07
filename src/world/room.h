#pragma once

#include "entities/character.h"
#include <vector>

class Entity;
class World;

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
    Entity* content;
    Entity* background;
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
    std::vector<Entity*> entities;
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
    Field* getEntrance(Direction dir);
    Room* getNeighbor(Direction dir);
    Field* getField(int x, int y);
    Entity* getEntity(int number);
    void addEntity(Entity* ent);
    void deleteEntity(int index);
    void updateEntityPos();
    void generate(Layout layout, bool forceUp = false, bool forceRight = false,
                  bool forceDown = false, bool forceLeft = false);
    bool generated();
};

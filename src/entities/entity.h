#pragma once

#include <iostream>

class Entity
{
protected:
    int posX, posY;
    bool blocking;
    char icon;
    std::string name;
    std::string desc;

public:
    virtual ~Entity(){};
    int getPosX();
    void setPosX(int value);
    int getPosY();
    void setPosY(int value);
    bool isBlocking();
    void setBlocking(bool value);
    char getIcon();
    void setIcon(char value);
    std::string getName();
    void setName(std::string value);
    std::string getDesc();
    void setDesc(std::string value);
};

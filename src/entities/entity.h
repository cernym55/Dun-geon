#pragma once

#include <iostream>

namespace Entities
{

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
    int getPosX() const;
    void setPosX(int value);
    int getPosY() const;
    void setPosY(int value);
    bool isBlocking() const;
    void setBlocking(bool value);
    char getIcon() const;
    void setIcon(char value);
    std::string getName() const;
    void setName(std::string value);
    std::string getDesc() const;
    void setDesc(std::string value);
};

} /* namespace Entities */
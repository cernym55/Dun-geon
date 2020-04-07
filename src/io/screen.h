#pragma once

#include "parser.h"
#include <iostream>

enum Mode
{
    WORLD,
    TEST,
    INVENTORY //, BATTLE, SKILLS, INVENTORY, MAP
};

class Room;

class Screen
{
private:
    Parser pars;
    Room* currentRoom;
    Mode mode;
    std::string getMapRow(int rowNumber);
    void printHUDRow(int rowNumber);
    char getFieldIcon(int fieldX, int fieldY);

public:
    Parser& parser();
    void printCenter(std::string str, int spaceWidth, bool secondPad);
    void mainMenu();
    void draw();
    void prompt();
    void clear();
    void drawInventory();
    void drawBattle();
    Mode getMode();
    void setMode(Mode m);
    Room* getCurrentRoom();
    void setCurrentRoom(Room* roomPtr);
};

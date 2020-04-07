#include "entities/player.h"
#include "ui/parser.h"
#include "ui/screen.h"
#include "worlds/room.h"
#include "worlds/world.h"

int main()
{
    int nextWorldNum = 1;
    Entities::Player* player = new Entities::Player();
    Worlds::World* world = new Worlds::World(nextWorldNum, player);
    world->getRoom(7, 7)->generate(Worlds::box, false, false, false, false);
    UI::Screen screen;
    screen.clear();
    screen.mainMenu();
    player->setPosX(world->getRoom(7, 7)->getDimX() / 2);
    player->setPosY(world->getRoom(7, 7)->getDimY() / 2);
    player->setBlocking(false);
    player->setName("Gref");
    player->setIcon(player->getName()[0]);
    player->setLastMove(Entities::up);
    player->getStats().level = 1;
    player->getStats().XP = 0;
    player->getStats().XPToNextLevel = 0;
    player->getStats().dun = 0;
    player->getStats().health = 10;
    player->getStats().healthMax = 10;
    player->getStats().mana = 5;
    player->getStats().manaMax = 10;
    player->getStats().vigor = 5;
    player->getStats().valor = 5;
    player->getStats().haste = 5;
    player->getStats().magic = 10;
    player->setCurrentRoom(world->getRoom(7, 7));
    screen.setMode(UI::WORLD);
    screen.setCurrentRoom(player->getCurrentRoom());
    screen.parser().setScreen(&screen);
    while (!screen.parser().quit())
    {
        // force screen to draw the room the player is in
        if (screen.getCurrentRoom() != player->getCurrentRoom())
        {
            screen.setCurrentRoom(player->getCurrentRoom());
        }
        player->getCurrentRoom()->updateEntityPos();
        if (screen.parser().getCmdQueue().empty())
        {
            screen.draw();
            screen.parser().readInput();
            screen.parser().parse();
            screen.parser().eval();
        }
        screen.parser().execCommand();
    }
    delete world;
    delete player;
    return 0;
}

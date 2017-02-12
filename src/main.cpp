/* *
 *
 * File:  main.cpp
 * Brief: Main function
 * 
 * Copyright (C) 2016-2017 Matyas Cerny <cerny-matyas(at)email.cz>
 *
 *
 * This file is part of Dun-geon.
 *
 * Dun-geon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dun-geon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Dun-geon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

#include "player.h"
#include "screen.h"
#include "world.h"
#include "room.h"
#include "parser.h"

int main() {
	int nextWorldNum = 1;
	Player *player = new Player();
	World *world = new World(nextWorldNum, player);
	world->getRoom(7,7)->generate(box, false, false, false, false);
	Screen screen;
	screen.clear();
	screen.mainMenu();
	player->setPosX(world->getRoom(7,7)->getDimX() / 2);
	player->setPosY(world->getRoom(7,7)->getDimY() / 2);
	player->setBlocking(false);
	player->setName("Gref");
	player->setIcon(player->getName()[0]);
	player->setLastMove(up);
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
	player->setCurrentRoom(world->getRoom(7,7));
	screen.setMode(WORLD);
	screen.setCurrentRoom(player->getCurrentRoom());
	screen.parser().setScreen(&screen);
	while (!screen.parser().quit()) {
		// force screen to draw the room the player is in
		if (screen.getCurrentRoom() != player->getCurrentRoom()) {
			screen.setCurrentRoom(player->getCurrentRoom());
		}
		player->getCurrentRoom()->updateEntityPos();
		if (screen.parser().getCmdQueue().empty()) {
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

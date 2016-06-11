#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <Windows.h>

#include "Tile.h"

extern Map map;
extern Map* currentMap;
extern Tile* cursorTile;
extern COORD cursorPosition;

extern std::vector<Tile> tiles;
extern std::vector<Item> items;
extern std::vector<Monster> monsters;
extern std::vector<Npc> npcs;

#include "Main.h"
#include "Tile.h"

using namespace std;

Tile::Tile() : type(0) {
	memset(&charInfo, 0, sizeof(COORD));
}

Item::Item() : life(0), strength(0), protection(0) {}

Monster::Monster() : life(0), strength(0), speed(0) {}

Transition::Transition() {
	positionDestinationMap.X = positionDestinationMap.Y = 0;
}

Map::Map() : currentListType(TILE_TYPE) {}

const CHAR_INFO Map::addNewBackground(CHAR_INFO image1, CHAR_INFO image2) {
	image1.Attributes = image1.Attributes << 12;
	image1.Attributes = image1.Attributes >> 12;
	image2.Attributes = image2.Attributes >> 4;
	image2.Attributes = image2.Attributes << 4;
	image1.Attributes = image1.Attributes | image2.Attributes;
	return image1;
}

const CHAR_INFO Map::addNewForeground(CHAR_INFO image1, CHAR_INFO image2) {
	image1.Attributes = image1.Attributes >> 4;
	image1.Attributes = image1.Attributes << 4;
	image2.Attributes = image2.Attributes << 12;
	image2.Attributes = image2.Attributes >> 12;
	image1.Attributes = image1.Attributes | image2.Attributes;
	return image1;
}

void Map::draw() {
	SMALL_RECT drawRect = { 0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1};
	COORD bufferSize = { MAP_WIDTH, MAP_HEIGHT };
	COORD zeroZero = {0, 0};
	DWORD dwResult = 0;
	char cursor[2] = "";
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	for (size_t i = 0; i < m_tiles.size(); i++) {
		screenBuffer[i] = m_tiles[i].getCharInfo();
	}
	WriteConsoleOutput(output, screenBuffer, bufferSize, zeroZero, &drawRect);
	if (cursorTile != NULL) {
		CHAR_INFO cursorInfo = cursorTile->getCharInfo();
		sprintf_s(cursor, "%c", cursorInfo.Char);
		WriteConsoleOutputCharacter(output, cursor, 1, cursorPosition, &dwResult);
		WriteConsoleOutputAttribute(output, &cursorInfo.Attributes, 1, cursorPosition, &dwResult);
	}
}

void Map::setDefault() {
	SMALL_RECT size = { 0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleWindowInfo(output, TRUE, &size);
	SetConsoleMode(input, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	m_tiles.clear();
	m_items.clear();
	m_monsters.clear();
	m_npcs.clear();
	m_transitions.clear();
	name = "";

	Tile defaultTile;
	CHAR_INFO defaultImage = { '#', FOREGROUND_GREEN };
	defaultTile.setCharInfo(defaultImage);
	for (int y = 0; y < MAP_HEIGHT - EDITOR_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			defaultTile.setIndex(x, y);
			m_tiles.push_back(defaultTile);
			screenBuffer[x + y * MAP_WIDTH] = defaultImage;
		}
	}

	for (int i = 0; i <= 286; i++) {
		WORD tileColor = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
		int index = i + (MAP_HEIGHT - EDITOR_HEIGHT + 1) * MAP_WIDTH;
		char tile = (char)i;
		if (i >= 255) {
			if (i <= 270) {
				tile = '@';
				tileColor = (WORD)(i - 255);
			}
			else {
				tile = ' ';
				tileColor = (WORD)(i - 255) << 4;
			}
		}
		screenBuffer[index].Char.AsciiChar = tile;
		screenBuffer[index].Attributes = tileColor;
		Tile tempTile;
		tempTile.setCharInfo(screenBuffer[index]);
		tempTile.setIndex(index, 0);
		tempTile.setType(TILE_TYPE);
		tiles.push_back(tempTile);
	}

	Tile tempCursorTile;
	tempCursorTile.setCharInfo(tiles[1].getCharInfo());
	tempCursorTile.setType(tiles[1].getType());
	tiles.push_back(tempCursorTile);
	cursorTile = &tiles[tiles.size() - 1];
}
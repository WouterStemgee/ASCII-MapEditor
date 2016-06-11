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

Npc::Npc() : life(0), strength(0), speed(0) {}

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
	for (size_t i = 0; i < m_items.size(); i++) {
		COORD index = m_items[i].getIndex();
		int slot = index.X + index.Y * MAP_WIDTH;
		screenBuffer[slot] = addNewBackground(m_items[i].getCharInfo(), screenBuffer[slot]);
	}
	for (size_t i = 0; i < m_monsters.size(); i++) {
		COORD index = m_monsters[i].getIndex();
		int slot = index.X + index.Y * MAP_WIDTH;
		screenBuffer[slot] = addNewBackground(m_monsters[i].getCharInfo(), screenBuffer[slot]);
	}
	for (size_t i = 0; i < m_npcs.size(); i++) {
		COORD index = m_npcs[i].getIndex();
		int slot = index.X + index.Y * MAP_WIDTH;
		screenBuffer[slot] = addNewBackground(m_npcs[i].getCharInfo(), screenBuffer[slot]);
	}
	for (size_t i = 0; i < m_transitions.size(); i++) {
		COORD index = m_transitions[i].getIndex();
		int slot = index.X + index.Y * MAP_WIDTH;
		screenBuffer[slot] = m_transitions[i].getCharInfo();
	}
	WriteConsoleOutput(output, screenBuffer, bufferSize, zeroZero, &drawRect);
	if (cursorTile != NULL) {
		CHAR_INFO cursorInfo = cursorTile->getCharInfo();
		sprintf_s(cursor, "%c", cursorInfo.Char);
		WriteConsoleOutputCharacter(output, cursor, 1, cursorPosition, &dwResult);
		WriteConsoleOutputAttribute(output, &cursorInfo.Attributes, 1, cursorPosition, &dwResult);
	}
	drawTileInfo();
}

void Map::drawTileInfo() {
	if (cursorTile == NULL) return;
	if (map.getCurrentListType() == TILE_TYPE || currentMap->getCurrentListType() == TRANSITION_TYPE) return;
	string nameTile;
	if (map.getCurrentListType() == ITEM_TYPE) {
		Item* item = (Item*)cursorTile;
		nameTile = item->getName();
	}
	else if (map.getCurrentListType() == MONSTER_TYPE) {
		Monster* monster = (Monster*)cursorTile;
		nameTile = monster->getName();
	}
	else if (map.getCurrentListType() == NPC_TYPE) {
		Npc* npc = (Npc*)cursorTile;
		nameTile = npc->getName();
	}
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, promptPosition);
	cout << "(Space Bar Cycles) Current tile: " << nameTile;
}

void Map::insertTile(Tile* tile, int x, int y) {
	char type = 't';
	if (currentMap->getCurrentListType() != TRANSITION_TYPE)
		type = tile->getType();
	if (type == ITEM_TYPE) {
		m_items.push_back(*(Item*)tile);
		m_items.back().setIndex(x, y);
	}
	else if (type == MONSTER_TYPE) {
		m_monsters.push_back(*(Monster*)tile);
		m_monsters.back().setIndex(x, y);
	}
	else if (type == NPC_TYPE) {
		m_npcs.push_back(*(Npc*)tile);
		m_npcs.back().setIndex(x, y);
	}
	else if (type == TRANSITION_TYPE) {
		CHAR_INFO image = { 'T', FOREGROUND_RED };
		Transition transition;
		transition.setCharInfo(image);
		m_transitions.push_back(transition);
		m_transitions.back().setIndex(x, y);
		m_transitions.back().setNameCurrentMap(currentMap->getName());
	}
	else {
		for (size_t i = 0; i < m_tiles.size(); i++) {
			COORD index = m_tiles[i].getIndex();
			if (index.X == x && index.Y == y) {
				m_tiles[i] = *tile;
				m_tiles[i].setIndex(index.X, index.Y);
				break;
			}
		}
	}
}

void Map::deleteTile(int mapX, int mapY) {
	if (currentListType == ITEM_TYPE) {
		for (int i = (int)m_items.size() - 1; i >= 0; i--) {
			if (m_items[i].getIndex().X == mapX && m_items[i].getIndex().Y == mapY) {
				m_items.erase(m_items.begin() + i);
				break;
			}
		}
	}
	else if (currentListType == MONSTER_TYPE) {
		for (int i = (int)m_monsters.size() - 1; i >= 0; i--) {
			if (m_monsters[i].getIndex().X == mapX && m_monsters[i].getIndex().Y == mapY) {
				m_monsters.erase(m_monsters.begin() + i);
				break;
			}
		}
	}
	else if (currentListType == NPC_TYPE) {
		for (int i = (int)m_npcs.size() - 1; i >= 0; i--) {
			if (m_npcs[i].getIndex().X == mapX && m_npcs[i].getIndex().Y == mapY) {
				m_npcs.erase(m_npcs.begin() + i);
				break;
			}
		}
	}
	else if (currentListType == TRANSITION_TYPE) {
		for (int i = (int)m_transitions.size() - 1; i >= 0; i--) {
			if (m_transitions[i].getIndex().X == mapX && m_transitions[i].getIndex().Y == mapY) {
				m_transitions.erase(m_transitions.begin() + i);
				break;
			}
		}
	}
}

void Map::setTileInfo(int type, char * file) {
	char name[MAX_PATH] = {0};
	FILE* fp = fopen(file, "r");
	if (!fp) {
		sprintf(file, "Unable to find tile info in %s", file);
		MessageBox(NULL, file, "Error", MB_OK);
		return;
	}
	while (!feof(fp)) {
		if (type == ITEM_TYPE) {
			Item tempItem;
			int life = 0, str = 0, protection = 0;
			CHAR_INFO image = { 'I', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
			fscanf(fp, "%s > life: %d strength: %d protection: %d\n", name, &life, &str, &protection);
			tempItem.setCharInfo(image);
			tempItem.setLife(life);
			tempItem.setStrength(str);
			tempItem.setProtection(protection);
			tempItem.setName(name);
			tempItem.setType(ITEM_TYPE);
			items.push_back(tempItem);
		}
		else if (type == MONSTER_TYPE) {
			Monster tempMonster;
			int life = 0, str = 0, speed = 0;
			CHAR_INFO image = { 0, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
			fscanf(fp, "%s > image: %c life: %d strength: %d speed: %d\n", name, &image.Char, &life, &str, &speed);
			tempMonster.setCharInfo(image);
			tempMonster.setLife(life);
			tempMonster.setStrength(str);
			tempMonster.setSpeed(speed);
			tempMonster.setName(name);
			tempMonster.setType(MONSTER_TYPE);
			monsters.push_back(tempMonster);
		}
		else if (type == NPC_TYPE) {
			Npc tempNpc;
			char message[MAX_MESSAGE_LENGTH] = { 0 };
			int life = 0, str = 0, speed = 0;
			CHAR_INFO image = { 1, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
			fscanf(fp, "%s > life: %d strength: %d speed: %d Message: ", name, &life, &str, &speed);
			fgets(message, MAX_MESSAGE_LENGTH, fp);
			if (strlen(message) > 1) {
				message[strlen(message) - 1] = '\0';
			}
			tempNpc.setCharInfo(image);
			tempNpc.setLife(life);
			tempNpc.setStrength(str);
			tempNpc.setSpeed(speed);
			tempNpc.setName(name);
			tempNpc.setMessage(message);
			tempNpc.setType(NPC_TYPE);
			npcs.push_back(tempNpc);
		}
	}
	fclose(fp);
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
	CHAR_INFO defaultImage = { (WCHAR)219, FOREGROUND_GREEN };
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
	CHAR_INFO tempCursor = { tiles[176].getCharInfo().Char, BACKGROUND_GREEN };
	tempCursorTile.setCharInfo(tempCursor);
	tempCursorTile.setType(tiles[176].getType());
	
	tiles.push_back(tempCursorTile);
	cursorTile = &tiles[tiles.size() - 1];
	setTileInfo(ITEM_TYPE, ITEM_INFO_FILE);
	setTileInfo(MONSTER_TYPE, MONSTER_INFO_FILE);
	setTileInfo(NPC_TYPE, NPC_INFO_FILE);
}


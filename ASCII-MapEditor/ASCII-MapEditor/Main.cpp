#include "Main.h"

using namespace std;

Map map;
Map* currentMap = NULL;
Tile* cursorTile = NULL;
COORD cursorPosition = { 0 };

vector<Tile> tiles;
vector<Item> items;
vector<Monster> monsters;
vector<Npc> npcs;

void init() {
	map.setDefault();
	map.setCurrentListType(TILE_TYPE);
	currentMap = &map;
}

void checkKeyboardInput(const INPUT_RECORD& inputRecord) {
	if (inputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
		exit(0);
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'I'){
		// TODO: Set listType to item tiles
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'M') {
		// TODO: Set listType to monster tiles
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'N') {
		// TODO: Set listType to npc tiles
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'T') {
		// TODO: Set listType to transition tiles
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == VK_SPACE) {
		// TODO: Cycle through list tiles
	}
}

void checkMouseInput(const INPUT_RECORD& inputRecord) {
	cursorPosition = inputRecord.Event.MouseEvent.dwMousePosition;
	if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
		if (cursorPosition.Y > MAP_HEIGHT -  EDITOR_HEIGHT) {
			CHAR_INFO chosenTile = map.getEditorSelection(cursorPosition.X, cursorPosition.Y);
			CHAR_INFO chosenCursorTile = chosenTile;
			if (cursorTile != NULL)
				chosenCursorTile = cursorTile->getCharInfo();
			if (cursorPosition.X >= 15 && cursorPosition.Y == (MAP_HEIGHT - 1)) {
				CHAR_INFO tile;
				if (cursorPosition.X > 30)
					tile = map.addNewBackground(chosenCursorTile, chosenTile);
				else
					tile = map.addNewForeground(chosenCursorTile, chosenTile);
				tiles[tiles.size() - 1].setCharInfo(tile);
			}
			else {
				chosenTile.Attributes = chosenCursorTile.Attributes;
				tiles[tiles.size() - 1].setCharInfo(chosenTile);
				cursorTile = &tiles[tiles.size() - 1];
				map.setCurrentListType(TILE_TYPE);
			}

		}
		else {
			// TODO: Add chosen tile into the map
		}
	}
	if (inputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
		// TODO: Remove current cursor tile or remove tile under cursor
	}
}

bool checkInput() {
	INPUT_RECORD inputRecord;
	DWORD events = 0;
	int bKeyDown = 0;

	HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
	ReadConsoleInput(input, &inputRecord, 1, &events);
	bKeyDown = inputRecord.Event.KeyEvent.bKeyDown;
	if (inputRecord.EventType == KEY_EVENT && bKeyDown) {
		checkKeyboardInput(inputRecord);
		return true;
	}
	if (inputRecord.EventType == MOUSE_EVENT) {
		checkMouseInput(inputRecord);
		return true;
	}
	return false;
}

int main() {
	init();
	map.draw();
	while (1) {
		if (checkInput()) {
			currentMap->draw();
		}
	}
	return 0;
}
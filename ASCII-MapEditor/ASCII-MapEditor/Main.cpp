#include "Main.h"

using namespace std;

Map map;
Map* currentMap = NULL;
Tile* cursorTile = NULL;
COORD cursorPosition = { 0 };
COORD promptPosition = { 0, MAP_HEIGHT - EDITOR_HEIGHT };

int itemIndex = 0;
int monsterIndex = 0;
int npcIndex = 0;

vector<Tile> tiles;
vector<Item> items;
vector<Monster> monsters;
vector<Npc> npcs;

void init() {
	map.setDefault();
	map.setCurrentListType(TILE_TYPE);
	currentMap = &map;
}

void loadOrSaveMap(bool load) {
	string mapName = "";
	char fileName[255] = { 0 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, promptPosition);
	cout << "\t\t\t\t\t\t\t\t\t\t";
	SetConsoleCursorPosition(output, promptPosition);
	HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(input, ENABLE_PROCESSED_INPUT | ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
	if (load)
		cout << "Enter a map to load: ";
	else
		cout << "Enter a map to Save: ";
	cin >> mapName;
	strcpy(fileName, mapName.c_str());
	if (load)
		map.load(fileName);
	else
		map.save(fileName);
	SetConsoleMode(input, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	map.draw();
}

void checkKeyboardInput(const INPUT_RECORD& inputRecord) {
	if (inputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
		exit(0);
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'I'){
		itemIndex = 0;
		cursorTile = &items[itemIndex];
		map.setCurrentListType(ITEM_TYPE);
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'M') {
		monsterIndex = 0;
		cursorTile = &monsters[monsterIndex];
		map.setCurrentListType(MONSTER_TYPE);
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'N') {
		npcIndex = 0;
		cursorTile = &npcs[npcIndex];
		map.setCurrentListType(NPC_TYPE);
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'T') {
		CHAR_INFO image = { 'T', FOREGROUND_RED };
		cursorTile->setCharInfo(image);
		cursorTile = &tiles[tiles.size() - 1];
		map.setCurrentListType(TRANSITION_TYPE);
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == VK_SPACE) {
		if (map.getCurrentListType() == ITEM_TYPE) {
			itemIndex = (itemIndex + 1) % (int)items.size();
			cursorTile = &items[itemIndex];
		}
		else if (map.getCurrentListType() == MONSTER_TYPE) {
			monsterIndex = (monsterIndex + 1) % (int)monsters.size();
			cursorTile = &monsters[monsterIndex];
		}
		else if (map.getCurrentListType() == NPC_TYPE) {
			npcIndex = (npcIndex + 1) % (int)npcs.size();
			cursorTile = &npcs[npcIndex];
		}
	}
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'L')
		loadOrSaveMap(true);
	else if (inputRecord.Event.KeyEvent.wVirtualKeyCode == 'S')
		loadOrSaveMap(false);
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
			if (cursorTile != NULL) {
				currentMap->insertTile(cursorTile, cursorPosition.X, cursorPosition.Y);
			}
		}
	}
	if (inputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
		if (cursorTile == NULL) {
			currentMap->deleteTile(cursorPosition.X, cursorPosition.Y);
		}
		else {
			cursorTile = NULL;
		}
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
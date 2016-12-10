#pragma once

#include "Main.h"								

#define MAP_WIDTH			80
#define MAP_HEIGHT			50
#define EDITOR_HEIGHT		5
#define MAX_NAME_LENGTH		50
#define MAX_MESSAGE_LENGTH	1000

#define MAP_EXTENSION		".map"
#define MONSTER_INFO_FILE	"Monsters.txt"
#define ITEM_INFO_FILE		"Items.txt"
#define NPC_INFO_FILE		"Npcs.txt"

#define TILE_TYPE			0
#define ITEM_TYPE			1
#define MONSTER_TYPE		2
#define NPC_TYPE			3
#define TRANSITION_TYPE		4

class Tile {
public:
	Tile();
	void setCharInfo(CHAR_INFO _charInfo) { charInfo = _charInfo; }
	void setType(char _type) { type = _type; }
	void setIndex(int _x, int _y) { index.X = _x; index.Y = _y; }
	void setSolid(bool _solid) { solid = _solid; }
	void setActionKey(int _actionKey) { actionKey = _actionKey; }
	char getType() { return type; }
	int getActionKey() { return actionKey; }
	bool isSolid() { return solid; }
	CHAR_INFO getCharInfo() { return charInfo; }
	COORD getIndex() { return index; }
protected:
	CHAR_INFO charInfo;
	char type;
	COORD index;
	bool solid;
	int actionKey;
};

class Item : public Tile {
public:
	Item();
	void setLife(int _life) { life = _life; }
	void setStrength(int _strength) { strength = _strength; }
	void setProtection(int _protection) { protection = _protection; }
	void setName(char *_name) { strcpy(name, _name); }
	int getLife() { return life; }
	int getStrength() { return strength; }
	int getProtection() { return protection; }
	char* getName() { return name; }
private:
	int life;
	int strength;
	int protection;
	char name[MAX_NAME_LENGTH];
};


class Monster : public Tile {
public:
	Monster();
	void setLife(int _life) { life = _life; }
	void setStrength(int _strength) { strength = _strength; }
	void setName(char *_name) { strcpy(name, _name); }
	void setSpeed(int _speed) { speed = _speed; }
	int getLife() { return life; }
	int getStrength() { return strength; }
	int getSpeed() { return speed; }
	char* getName() { return name; }
private:
	int life;
	int strength;
	int speed;
	char name[MAX_NAME_LENGTH];
};

class Npc : public Tile {
public:
	Npc();
	void setLife(int _life) { life = _life; }
	void setStrength(int _strength) { strength = _strength; }
	void setSpeed(int _speed) { speed = _speed; }
	void setName(char *_name) { strcpy(name, _name); }
	void setMessage(char *_message) { strncpy(message, _message, MAX_MESSAGE_LENGTH); }
	int getLife() { return life; }
	int getStrength() { return strength; }
	int getSpeed() { return speed; }
	char *getName() { return name; }
	char *getMessage() { return message; }
private:
	int life;
	int strength;
	int speed;
	char name[MAX_NAME_LENGTH];
	char message[MAX_MESSAGE_LENGTH];
};

class Transition : public Tile {
public:
	Transition();
	void setNameCurrentMap(char *_nameCurentMap) { strcpy(nameCurrentMap, _nameCurentMap); }
	void setNameDestinationMap(char *_nameDestinationMap) { strcpy(nameDestinationMap, _nameDestinationMap); }
	void setPositionDestinationMap(COORD _positionDestinationMap) { positionDestinationMap = _positionDestinationMap; }
	char *getNameCurrentMap() { return nameCurrentMap; }
	char *getNameDestinationMap() { return nameDestinationMap; }
	COORD getPositionDestinationMap() { return positionDestinationMap; }
private:
	char nameCurrentMap[MAX_PATH];
	char nameDestinationMap[MAX_PATH];
	COORD positionDestinationMap;
};

class Map {
public:
	Map();
	void setDefault();
	void draw();	
	void load(char* fileName);
	void save(char* fileName);
	void setCurrentListType(int _currentListType) { currentListType = _currentListType; }
	void drawTileInfo();
	void insertTile(Tile* tile, int x, int y);
	void deleteTile(int mapX, int mapY);
	void setTileInfo(int type, char* fileName);	
	void setTransitions(char* fileName, Transition* transition);
	void deleteBlankTransitions();
	void setCollisionTile(bool isSolid, int x, int y);
	char* getName() { return name; }
	int getCurrentListSize();
	int getCurrentListType() { return currentListType; }
	Tile* getCurrentListTile(int index);
	Tile* findMapTile(int mapX, int mapY);
	CHAR_INFO getEditorSelection(int x, int y) { return screenBuffer[x + y * MAP_WIDTH]; }
	CHAR_INFO addNewBackground(CHAR_INFO image1, CHAR_INFO image2);
	CHAR_INFO addNewForeground(CHAR_INFO image1, CHAR_INFO image2);
private:
	CHAR_INFO screenBuffer[MAP_WIDTH * MAP_HEIGHT];
	char name[MAX_PATH];
	int currentListType;
	std::vector<Tile> m_tiles;
	std::vector<Item> m_items;
	std::vector<Monster> m_monsters;
	std::vector<Npc> m_npcs;
	std::vector<Transition> m_transitions;
};
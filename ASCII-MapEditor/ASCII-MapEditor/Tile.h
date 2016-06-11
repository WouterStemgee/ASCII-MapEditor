#pragma once

#include "Main.h"

#define MAP_WIDTH			120
#define MAP_HEIGHT			75
#define EDITOR_HEIGHT		4
#define MAP_EXTENSION		".map"
#define MAX_MESSAGE_LENGTH	1000

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
	const CHAR_INFO getCharInfo() { return charInfo; }
	const char getType() { return type; }
	const COORD getIndex() { return index; }
protected:
	CHAR_INFO charInfo;
	char type;
	COORD index;
};

class Item : public Tile {
public:
	Item();
	void setLife(int _life) { life = _life; }
	void setStrength(int _strength) { strength = _strength; }
	void setProtection(int _protection) { protection = _protection; }
	void setName(const std::string& _name) { name = _name; }
	const int getLife() { return life; }
	const int getStrength() { return strength; }
	const int getProtection() { return protection; }
	const std::string& getName() { return name; }
private:
	int life;
	int strength;
	int protection;
	std::string name;
};

class Monster : public Tile {
public:
	Monster();
	void setLife(int _life) { life = _life; }
	void setStrength(int _strength) { strength = _strength; }
	void setName(const std::string& _name) { name = _name; }
	void setSpeed(int _speed) { speed = _speed; }
	const int getLife() { return life; }
	const int getStrength() { return strength; }
	const std::string& getName() { return name; }
	const int getSpeed() { return speed; }
private:
	int life;
	int strength;
	int speed;
	std::string name;
};

class Npc : public Tile {
public:
	Npc();
	void setLife(int _life) { life = _life; }
	void setStrength(int _strength) { strength = _strength; }
	void setSpeed(int _speed) { speed = _speed; }
	void setName(const std::string& _name) { name = _name; }
	void setMessage(const std::string& _message) { message = _message; }
	const int getLife() { return life; }
	const int getStrength() { return strength; }
	const int getSpeed() { return speed; }
	const std::string& getName() { return name; }
	const std::string& getMessage() { return name; }
private:
	int life;
	int strength;
	int speed;
	std::string name;
	std::string message;
};

class Transition : public Tile {
public:
	Transition();
	void setNameCurrentMap(const std::string& _nameCurentMap) { nameCurrentMap = _nameCurentMap; }
	void setNameDestinationMap(const std::string& _nameDestinationMap) { nameDestinationMap = _nameDestinationMap; }
	void setPositionDestinationMap(const COORD& _positionDestinationMap) { positionDestinationMap = _positionDestinationMap; }
	const std::string& getNameCurrentMap() { return nameCurrentMap; }
	const std::string& getNameDestinationMap() { return nameDestinationMap; }
	const COORD& getPositionDestinationMap() { return positionDestinationMap; }
private:
	std::string nameCurrentMap;
	std::string nameDestinationMap;
	COORD positionDestinationMap;
};

class Map {
public:
	Map();
	void setDefault();
	void draw();
	void setCurrentListType(int _currentListType) { currentListType = _currentListType; }
	void drawTileInfo();
	void insertTile(Tile* tile, int x, int y);
	void deleteTile(int mapX, int mapY);
	void setTileInfo(int type, char* file);
	const std::string& getName() { return name; }
	const int getCurrentListType() { return currentListType; }
	const CHAR_INFO getEditorSelection(int x, int y) { return screenBuffer[x + y * MAP_WIDTH]; }
	const CHAR_INFO addNewBackground(CHAR_INFO image1, CHAR_INFO image2);
	const CHAR_INFO addNewForeground(CHAR_INFO image1, CHAR_INFO image2);
private:
	CHAR_INFO screenBuffer[MAP_WIDTH * MAP_HEIGHT];
	std::string name;
	int currentListType;
	std::vector<Tile> m_tiles;
	std::vector<Item> m_items;
	std::vector<Monster> m_monsters;
	std::vector<Npc> m_npcs;
	std::vector<Transition> m_transitions;
};
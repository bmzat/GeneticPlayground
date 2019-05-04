#pragma once
#include <vector>
#include "MapItem.h"
#include "MapTile.h"

class WorldMap
{
public:
	WorldMap();
	~WorldMap();
	MapTile **tiles;
	int sizeX;
	int sizeY;
	std::vector<MapItem*> items;
private:
public:

	int setSize(int x, int y);
//	int findNearesItem();
	int findNearestItem(int x, int y);
};


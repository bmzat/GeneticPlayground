#include "stdafx.h"
#include "WorldMap.h"
#include "stdlib.h"
#include <set>
#include <algorithm>
#include <utility>

WorldMap::WorldMap()
{
	sizeX = 0;
	sizeY = 0;
	tiles = NULL;
}


WorldMap::~WorldMap()
{
	for (int n = 0; n < (sizeX*sizeY); n++)delete tiles[n];
	free(tiles);
}


int WorldMap::setSize(int x, int y)
{
	sizeX = x;
	sizeY = y;
	tiles = (MapTile**)calloc(x*y,sizeof(MapTile*));
	for (int px = 0; px < sizeX; px++) {
		for (int py = 0; py < sizeY; py++) {
			MapTile *t = new MapTile();
			float *c = t->getColor();
			tiles[px*x + py] = t;
			c[0] = 0.3f;
			c[1] = 0.3f;
			c[2] = 0.3f;
#if 0 //checkerboard
			if (px % 2) {
				c[0] = float(py % 2)+0.4f;
				c[1] = float(py % 2) + 0.4f;
				c[2] = float(py % 2) + 0.4f;
			}
			else {
				c[0] = float((py+1) % 2) + 0.4f;
				c[1] = float((py+1) % 2) + 0.4f;
				c[2] = float((py+1) % 2) + 0.4f;
			}
#endif
		}
	}
	for (int n = 0; n < 100; n++) {
		MapItem* itm = new MapItem();
		itm->pX = rand() % sizeX;
		itm->pY = rand() % sizeY;
		items.push_back(itm);
	}
	return 0;
}


//int WorldMap::findNearesItem()
//{
//	// TODO: Fügen Sie hier Ihren Implementierungscode ein..
//	return 0;
//}


int WorldMap::findNearestItem(int x, int y)
{
	std::vector<std::pair<int, int>> pairs;

	for (unsigned int n = 0; n < items.size(); n++) {
		std::pair<int, int> p;
		int distance = (items[n]->pX - x)*(items[n]->pX - x) + (items[n]->pY - y)*(items[n]->pY - y);
		p.first = n;
		p.second = distance;
		pairs.push_back(p);
	}
	

	sort(pairs.begin(), pairs.end(), [=](std::pair<int, int>& a, std::pair<int, int>& b)
	{
		return a.second < b.second;
	}
	);
	int ret = pairs.front().first;
	return ret;
}


void WorldMap::reitemize(int n)
{
	for (MapItem* i : items) {
		delete i;
	}
	items.clear();

	for (int i= 0; i< n; i++) {
		MapItem* itm = new MapItem();
		itm->pX = rand() % sizeX;
		itm->pY = rand() % sizeY;
		items.push_back(itm);
	}
	return ;
}


void WorldMap::removeItem(int idx)
{
	if (items[idx])
		//delete items[idx];
	items.erase(items.begin() + idx);
	return ;
}

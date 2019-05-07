#include "stdafx.h"
#include "Population.h"
#include "stdlib.h"
#include <set>
#include <algorithm>
#include <utility>

Population::Population()
{
	step = 0;
}


Population::~Population()
{
}


int Population::spawn(int n, WorldMap* map)
{
	this->map = map;
	for (Creature* i : pop) {
		delete i;
	}
	pop.clear();

	for (int i = 0; i< n; i++) {
		Creature* itm = new Creature();
		itm->pX = float(rand() % map->sizeX);
		itm->pY = float(rand() % map->sizeY);
		pop.push_back(itm);
	}
	return 0;
}


int Population::simulateStep()
{
	step++;
	for (unsigned int n = 0; n < pop.size(); n++) {
		bool valid = true;
		int d = map->findNearestItem(pop[n]->pX, pop[n]->pY);
		do {
			valid = true;
			int dir = rand() % 4;
			float dist = (map->items[d]->pX - pop[n]->pX)*(map->items[d]->pX - pop[n]->pX) + (map->items[d]->pY - pop[n]->pY)*(map->items[d]->pY - pop[n]->pY);
			if (dist < 30) {
				float dx = (map->items[d]->pX - pop[n]->pX);
				float dy = (map->items[d]->pY - pop[n]->pY);
				if (abs(dx) >= abs(dy)) {
					pop[n]->pX += (dx > 0) ? 1 : -1;
				}
				else {
					pop[n]->pY += (dy > 0) ? 1 : -1;
				}
			}else{
			switch (dir) {
				case 0: {
					if (pop[n]->pX > 0) {
						pop[n]->pX--;
					}
					else {
						valid = false;
					}
				}break;
				case 1: {
					if (pop[n]->pX < map->sizeX-1) {
						pop[n]->pX++;
					}
					else {
						valid = false;
					}
				}break;
				case 2: {
					if (pop[n]->pY > 0) {
						pop[n]->pY--;
					}
					else {
						valid = false;
					}
				}break;
				case 3: {
					if (pop[n]->pY < map->sizeY - 1) {
						pop[n]->pY++;
					}
					else {
						valid = false;
					}
				}break;
				default:
					valid = false;
			}
			}
			
		} while (!valid);

		//int d = map->findNearestItem(pop[n]->pX, pop[n]->pY);
		if ((map->items[d]->pX== pop[n]->pX)&& (map->items[d]->pY == pop[n]->pY)) {
			pop[n]->stamina += 10;
			pop[n]->food++;
			if (pop[n]->food >= 2) {
				Creature *c = new Creature();
				c->pX = pop[n]->pX;
				c->pY = pop[n]->pY;
				pop[n]->food = 0;
				pop.push_back(c);
				
			}
			map->removeItem(d);
		}

		pop[n]->stamina--;

	}
	pop.erase(
		std::remove_if(
			pop.begin(),
			pop.end(),
			[](Creature* const & p) { bool bRet = false; if (p->stamina <= 0) { delete p; bRet = true; }
	return bRet; }
		),
		pop.end()
	);

	if (!(step % 20))map->reitemize(250);
	// TODO: Fügen Sie hier Ihren Implementierungscode ein..
	return 0;
}

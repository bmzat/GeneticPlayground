#pragma once
#include <vector>
#include "Creature.h"
#include "WorldMap.h"

class Population
{
public:
	Population();
	~Population();
	std::vector<Creature*> pop;
	WorldMap *map;
	int spawn(int n, WorldMap* map);
	int simulateStep();
	int step;
};


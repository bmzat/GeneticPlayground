#include "stdafx.h"
#include "Creature.h"


Creature::Creature()
{
	pX = 0;
	pY = 0;
	food = 0;
	stamina = 20;
}


Creature::~Creature()
{
}


void Creature::simulateStep()
{
}

void Creature::checkAffinity(MapItem * item)
{
}

void Creature::checkAffinity(Creature * item)
{
}

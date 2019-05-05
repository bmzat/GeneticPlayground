#pragma once
class MapItem;
class Creature;
class Gene
{
public:
	Gene();
	~Gene();
	virtual int exprimate(Creature *pCreature);
//	virtual float getFoodAttraction();
	virtual float getMateAttraction(Creature *pCreature);
	virtual float getFoodAttraction(MapItem *pItem);
	virtual float getFoodAttraction(Creature *pCreature);
	virtual Gene* biologicCopy();
};


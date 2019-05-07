#pragma once
#include <math.h>
#include <string>
#include <map>

class MapItem;

class Creature
{
public:
	Creature();
	~Creature();
	float pX;
	float pY;
	int food;
	int stamina;
	
	virtual void simulateStep();
	virtual void checkAffinity(MapItem* item);
	virtual void checkAffinity(Creature* item);
	float checkTrait(std::string name);
	void modifyTrait(std::string name, float value);
private:
	
	std::map<std::string, float> traits;
};


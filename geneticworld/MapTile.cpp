#include "stdafx.h"
#include "MapTile.h"


MapTile::MapTile()
{
	color[0] = 0.0;
	color[1] = 0.0;
	color[2] = 0.0;
}


MapTile::~MapTile()
{
}


float* MapTile::getColor()
{
	return color;
}

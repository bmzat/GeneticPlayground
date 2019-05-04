#pragma once
class MapTile
{
public:
	MapTile();
	~MapTile();
private:
	float color[3];
public:
	float* getColor();
};


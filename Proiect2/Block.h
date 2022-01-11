#pragma once
#include <cmath>
#include <iostream>
#include <string>

const int BLOCK_SIZE = 30;
class Block
{
public:
	int x, y, z, type;
	Block(int _x, int _y, int _z, int type);
	float dist(float x, float y, float z);
	int hit_by_ray(float x, float y, float z, float dx, float dy, float dz);
};


#include "Block.h"

Block::Block(int x, int y, int z, int type) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->type = type;
}

float Block::dist(float x, float y, float z)
{

	float thisx = this->x * BLOCK_SIZE;
	float thisy = this->y * BLOCK_SIZE;		
	float thisz = this->z * BLOCK_SIZE;
	return sqrt(
		(x - thisx) * (x - thisx) +
		(y - thisy) * (y - thisy) +
		(z - thisz) * (z - thisz)
	);

}

int Block::hit_by_ray(float x, float y, float z, float dx, float dy, float dz)
{
	float d_frac_x = 1.0f / dx;
	float d_frac_y = 1.0f / dy;
	float d_frac_z = 1.0f / dz;

	float left_bot_x =	this->x * BLOCK_SIZE - BLOCK_SIZE / 2;
	float left_bot_y =	this->y * BLOCK_SIZE - BLOCK_SIZE / 2;
	float left_bot_z =	this->z * BLOCK_SIZE - BLOCK_SIZE / 2;
	float right_top_x = this->x * BLOCK_SIZE + BLOCK_SIZE / 2;
	float right_top_y = this->y * BLOCK_SIZE + BLOCK_SIZE / 2;
	float right_top_z = this->z * BLOCK_SIZE + BLOCK_SIZE / 2;

	float t[] = {
		(left_bot_x - x) * d_frac_x,
		(right_top_x - x) * d_frac_x,
		(left_bot_y - y) * d_frac_y,
		(right_top_y - y) * d_frac_y,
		(left_bot_z - z) * d_frac_z,
		(right_top_z - z) * d_frac_z
	};
	
	float t_max = std::min(std::min(std::max(t[0], t[1]), std::max(t[2], t[3])), std::max(t[4], t[5]));
	float t_min = std::max(std::max(std::min(t[0], t[1]), std::min(t[2], t[3])), std::min(t[4], t[5]));

	if (t_max < 0) {
		return 0;
	}

	if (t_min > t_max) {
		return 0;
	}

	for (int i = 0; i < 6; i++) {
		if (t_min == t[i]) {
			return i + 1;
		}
	}
	return 1;
}

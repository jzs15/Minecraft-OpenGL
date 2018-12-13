#pragma once
#include "global.h"
#include "chunk.h"
#include <ctime>

class World
{
public:
	World();
	~World();

	uint8_t get(int x, int y, int z) const;
	void set(int x, int y, int z, uint8_t type);
	void render(const glm::mat4 &pv);

private:
	Chunk *c[SCX][SCY][SCZ];
	time_t seed;
};


#pragma once
#include "global.h"

struct byte4 {
	uint8_t x, y, z, w;
	byte4() {}
	byte4(uint8_t x, uint8_t y, uint8_t z, uint8_t w) : x(x), y(y), z(z), w(w) {}
};

class Chunk
{
public:
	Chunk();
	Chunk(int x, int y, int z);
	~Chunk();

	uint8_t get(int x, int y, int z) const;
	bool isblocked(int x1, int y1, int z1, int x2, int y2, int z2);
	void set(int x, int y, int z, uint8_t type);
	static float noise2d(float x, float y, int seed, int octaves, float persistence);
	static float noise3d_abs(float x, float y, float z, int seed, int octaves, float persistence);
	void noise(int seed);
	void update();
	void render();

public:
	Chunk *left, *right, *below, *above, *front, *back;
	int ax, ay, az;
	bool initialized;

private:
	uint8_t blk[CX][CY][CZ];

	int slot;
	GLuint vbo;
	int elements;
	time_t lastused;
	bool changed;
	bool noised;
};

static const int transparent[44] = { 2, 0, 0, 0, 0, 0, 0, 4,
									 0, 3, 0, 0, 0, 0, 0, 0,
									 0, 0, 0, 0, 0, 0, 0, 0,
									 1, 0, 1, 0, 0, 0, 0, 0,
									 0, 0, 0, 1, 1, 1, 1, 1,
									 1, 1, 1, 1 };
static Chunk *chunk_slot[CHUNKSLOTS];
#include "chunk.h"

Chunk::Chunk() :ax(0), ay(0), az(0) {
	memset(blk, 0, sizeof blk);
	left = right = below = above = front = back = 0;
	lastused = now;
	slot = 0;
	changed = true;
	initialized = false;
	noised = false;
}

Chunk::Chunk(int x, int y, int z) : ax(x), ay(y), az(z) {
	memset(blk, 0, sizeof blk);
	left = right = below = above = front = back = 0;
	lastused = now;
	slot = 0;
	changed = true;
	initialized = false;
	noised = false;
}


Chunk::~Chunk()
{
}

uint8_t Chunk::get(int x, int y, int z) const {
	if (x < 0)
		return left ? left->blk[x + CX][y][z] : 0;
	if (x >= CX)
		return right ? right->blk[x - CX][y][z] : 0;
	if (y < 0)
		return below ? below->blk[x][y + CY][z] : 0;
	if (y >= CY)
		return above ? above->blk[x][y - CY][z] : 0;
	if (z < 0)
		return front ? front->blk[x][y][z + CZ] : 0;
	if (z >= CZ)
		return back ? back->blk[x][y][z - CZ] : 0;
	return blk[x][y][z];
}

bool Chunk::isblocked(int x1, int y1, int z1, int x2, int y2, int z2) {
	// Invisible blocks are always "blocked"
	if (!blk[x1][y1][z1])
		return true;

	if (transparent[get(x1, y1, z1)] == 5)
		return false;

	// Leaves do not block any other block, including themselves
	if (transparent[get(x2, y2, z2)] == 1 || transparent[get(x2, y2, z2)] == 5)
		return false;

	// Non-transparent blocks always block line of sight
	if (!transparent[get(x2, y2, z2)])
		return true;

	// Otherwise, LOS is only blocked by blocks if the same transparency type
	return transparent[get(x2, y2, z2)] == transparent[blk[x1][y1][z1]];
}

void Chunk::set(int x, int y, int z, uint8_t type) {
	// If coordinates are outside this chunk, find the right one.
	if (x < 0) {
		if (left)
			left->set(x + CX, y, z, type);
		return;
	}
	if (x >= CX) {
		if (right)
			right->set(x - CX, y, z, type);
		return;
	}
	if (y < 0) {
		if (below)
			below->set(x, y + CY, z, type);
		return;
	}
	if (y >= CY) {
		if (above)
			above->set(x, y - CY, z, type);
		return;
	}
	if (z < 0) {
		if (front)
			front->set(x, y, z + CZ, type);
		return;
	}
	if (z >= CZ) {
		if (back)
			back->set(x, y, z - CZ, type);
		return;
	}

	// Change the block
	blk[x][y][z] = type;
	changed = true;

	// When updating blocks at the edge of this chunk,
	// visibility of blocks in the neighbouring chunk might change.
	if (x == 0 && left)
		left->changed = true;
	if (x == CX - 1 && right)
		right->changed = true;
	if (y == 0 && below)
		below->changed = true;
	if (y == CY - 1 && above)
		above->changed = true;
	if (z == 0 && front)
		front->changed = true;
	if (z == CZ - 1 && back)
		back->changed = true;
}

float Chunk::noise2d(float x, float y, int seed, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for (int i = 0; i < octaves; i++) {
		sum += strength * glm::simplex(glm::vec2(x, y) * scale);
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

float Chunk::noise3d_abs(float x, float y, float z, int seed, int octaves, float persistence) {
	float sum = 0;
	float strength = 1.0;
	float scale = 1.0;

	for (int i = 0; i < octaves; i++) {
		sum += strength * fabs(glm::simplex(glm::vec3(x, y, z) * scale));
		scale *= 2.0;
		strength *= persistence;
	}

	return sum;
}

void Chunk::noise(int seed) {
	if (noised)
		return;
	else
		noised = true;

	for (int x = 0; x < CX; x++) {
		for (int z = 0; z < CZ; z++) {
			// Land height
			float n = noise2d((x + ax * CX) / 256.0, (z + az * CZ) / 256.0, seed, 5, 0.8) * 4;
			int h = n * 2;
			int y = 0;

			// Land blocks
			for (y = 0; y < CY; y++) {
				// Are we above "ground" level?
				if (y + ay * CY >= h) {
					// If we are not yet up to sea level, fill with water blocks
					if (y + ay * CY < SEALEVEL) {
						blk[x][y][z] = 9;
						continue;
						// Otherwise, we are in the air
					}
					else {
						// A tree!
						if (get(x, y - 1, z) == 2) {
							int ran = rand();
							if ((ran <= 255))
							{
								// Trunk
								h = (rand() & 0x3) + 3;
								for (int i = 0; i < h; i++)
									set(x, y + i, z, 25);

								// Leaves
								for (int ix = -3; ix <= 3; ix++) {
									for (int iy = -3; iy <= 3; iy++) {
										for (int iz = -3; iz <= 3; iz++) {
											if (ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !get(x + ix, y + h + iy, z + iz))
												set(x + ix, y + h + iy, z + iz, 24);
										}
									}
								}
							}
							else if (ran <= 2500)
							{
								set(x, y, z, 42);
							}
							else if (ran <= 3500)
							{
								int flower = (rand() % 5) + 35;
								set(x, y, z, flower);
							}
						}
						break;
					}
				}

				// Random value used to determine land type
				float r = noise3d_abs((x + ax * CX) / 16.0, (y + ay * CY) / 16.0, (z + az * CZ) / 16.0, -seed, 2, 1);

				// Sand layer
				if (n + r * 5 < 4)
					blk[x][y][z] = 6;
				// Dirt layer, but use grass blocks for the top
				else if (n + r * 5 < 8)
					blk[x][y][z] = (h < SEALEVEL || y + ay * CY < h - 1) ? 3 : 2;
				// Rock layer
				else if (r < 1.25)
					blk[x][y][z] = 1;
				// Sometimes, ores!
				else
					blk[x][y][z] = 10;
			}
		}
	}
	changed = true;
}

void Chunk::update() {
	byte4 vertex[CX * CY * CZ * 18];
	int i = 0;
	int merged = 0;
	bool vis = false;;

	// View from negative x

	for (int x = CX - 1; x >= 0; x--) {
		for (int y = 0; y < CY; y++) {
			for (int z = 0; z < CZ; z++) {
				// Line of sight blocked?
				if (isblocked(x, y, z, x - 1, y, z)) {
					vis = false;
					continue;
				}
				uint8_t side = blk[x][y][z];
				uint8_t type = 0;
				if (side >= 35 && side <= 42)
				{
					type = 1;
				}
				else if (side == 43)
				{
					type = 3;
				}

				// Same block as previous one? Extend it.
				if (vis && z != 0 && blk[x][y][z] == blk[x][y][z - 1]) {
					vertex[i - 5] = byte4(x, y, z + 1, side, type);
					vertex[i - 2] = byte4(x, y, z + 1, side, type);
					vertex[i - 1] = byte4(x, y + 1, z + 1, side, type);
					merged++;
					// Otherwise, add a new quad.
				}
				else {
					vertex[i++] = byte4(x, y, z, side, type);
					vertex[i++] = byte4(x, y, z + 1, side, type);
					vertex[i++] = byte4(x, y + 1, z, side, type);
					vertex[i++] = byte4(x, y + 1, z, side, type);
					vertex[i++] = byte4(x, y, z + 1, side, type);
					vertex[i++] = byte4(x, y + 1, z + 1, side, type);
				}

				vis = true;
			}
		}
	}

	// View from positive x

	for (int x = 0; x < CX; x++) {
		for (int y = 0; y < CY; y++) {
			for (int z = 0; z < CZ; z++) {
				if (isblocked(x, y, z, x + 1, y, z)) {
					vis = false;
					continue;
				}
				uint8_t side = blk[x][y][z];
				uint8_t type = 0;
				if (side >= 35 && side <= 42)
				{
					vis = true;
					continue;
				}
				else if (side == 43)
				{
					type = 4;
				}

				if (vis && z != 0 && blk[x][y][z] == blk[x][y][z - 1]) {
					vertex[i - 4] = byte4(x + 1, y, z + 1, side, type);
					vertex[i - 2] = byte4(x + 1, y + 1, z + 1, side, type);
					vertex[i - 1] = byte4(x + 1, y, z + 1, side, type);
					merged++;
				}
				else {
					vertex[i++] = byte4(x + 1, y, z, side, type);
					vertex[i++] = byte4(x + 1, y + 1, z, side, type);
					vertex[i++] = byte4(x + 1, y, z + 1, side, type);
					vertex[i++] = byte4(x + 1, y + 1, z, side, type);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, side, type);
					vertex[i++] = byte4(x + 1, y, z + 1, side, type);
				}
				vis = true;
			}
		}
	}

	// View from negative y

	for (int x = 0; x < CX; x++) {
		for (int y = CY - 1; y >= 0; y--) {
			for (int z = 0; z < CZ; z++) {
				if (isblocked(x, y, z, x, y - 1, z)) {
					vis = false;
					continue;
				}
				uint8_t bottom = blk[x][y][z];
				uint8_t type = 0;
				if (bottom == 2) 
				{
					bottom = 3;
				}
				else if (bottom == 25)
				{
					bottom = 57;
				}
				else if (bottom == 27)
				{
					bottom = 58;
				}
				else if (bottom == 28)
				{
					bottom = 59;
				}
				else if (bottom >= 35 && bottom <= 42)
				{
					vis = true;
					continue;
				}
				else if (bottom == 43)
				{
					bottom = 48;
				}

				if (vis && z != 0 && blk[x][y][z] == blk[x][y][z - 1]) {
					vertex[i - 4] = byte4(x, y, z + 1, bottom + 128, type);
					vertex[i - 2] = byte4(x + 1, y, z + 1, bottom + 128, type);
					vertex[i - 1] = byte4(x, y, z + 1, bottom + 128, type);
					merged++;
				}
				else {
					vertex[i++] = byte4(x, y, z, bottom + 128, type);
					vertex[i++] = byte4(x + 1, y, z, bottom + 128, type);
					vertex[i++] = byte4(x, y, z + 1, bottom + 128, type);
					vertex[i++] = byte4(x + 1, y, z, bottom + 128, type);
					vertex[i++] = byte4(x + 1, y, z + 1, bottom + 128, type);
					vertex[i++] = byte4(x, y, z + 1, bottom + 128, type);
				}
				vis = true;
			}
		}
	}

	// View from positive y

	for (int x = 0; x < CX; x++) {
		for (int y = 0; y < CY; y++) {
			for (int z = 0; z < CZ; z++) {
				if (isblocked(x, y, z, x, y + 1, z)) {
					vis = false;
					continue;
				}
				uint8_t top = blk[x][y][z];
				uint8_t type = 0;
				if (top == 2) {
					top = 56;
				}
				else if (top == 25)
				{
					top = 57;
				}
				else if (top == 27)
				{
					top = 58;
				}
				else if (top == 28)
				{
					top = 60;
				}
				else if (top >= 35 && top <= 42)
				{
					vis = true;
					continue;
				}
				else if (top == 43)
				{
					type = 5;
					top = 49;
				}

				if (vis && z != 0 && blk[x][y][z] == blk[x][y][z - 1]) {
					vertex[i - 5] = byte4(x, y + 1, z + 1, top + 128, type);
					vertex[i - 2] = byte4(x, y + 1, z + 1, top + 128, type);
					vertex[i - 1] = byte4(x + 1, y + 1, z + 1, top + 128, type);
					merged++;
				}
				else {
					vertex[i++] = byte4(x, y + 1, z, top + 128, type);
					vertex[i++] = byte4(x, y + 1, z + 1, top + 128, type);
					vertex[i++] = byte4(x + 1, y + 1, z, top + 128, type);
					vertex[i++] = byte4(x + 1, y + 1, z, top + 128, type);
					vertex[i++] = byte4(x, y + 1, z + 1, top + 128, type);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, top + 128, type);
				}
				vis = true;
			}
		}
	}

	// View from negative z

	for (int x = 0; x < CX; x++) {
		for (int z = CZ - 1; z >= 0; z--) {
			for (int y = 0; y < CY; y++) {
				if (isblocked(x, y, z, x, y, z - 1)) {
					vis = false;
					continue;
				}
				uint8_t side = blk[x][y][z];
				uint8_t type = 0;
				if (side >= 35 && side <= 42)
				{
					type = 2;
				}
				else if (side == 43)
				{
					type = 6;
				}

				if (vis && y != 0 && blk[x][y][z] == blk[x][y - 1][z]) {
					vertex[i - 5] = byte4(x, y + 1, z, side, type);
					vertex[i - 3] = byte4(x, y + 1, z, side, type);
					vertex[i - 2] = byte4(x + 1, y + 1, z, side, type);
					merged++;
				}
				else {
					vertex[i++] = byte4(x, y, z, side, type);
					vertex[i++] = byte4(x, y + 1, z, side, type);
					vertex[i++] = byte4(x + 1, y, z, side, type);
					vertex[i++] = byte4(x, y + 1, z, side, type);
					vertex[i++] = byte4(x + 1, y + 1, z, side, type);
					vertex[i++] = byte4(x + 1, y, z, side, type);
				}
				vis = true;
			}
		}
	}

	// View from positive z

	for (int x = 0; x < CX; x++) {
		for (int z = 0; z < CZ; z++) {
			for (int y = 0; y < CY; y++) {
				if (isblocked(x, y, z, x, y, z + 1)) {
					vis = false;
					continue;
				}
				uint8_t side = blk[x][y][z];
				uint8_t type = 0;
				if (side >= 35 && side <= 42)
				{
					vis = true;
					continue;
				}
				else if (side == 43)
				{
					type = 7;
				}

				if (vis && y != 0 && blk[x][y][z] == blk[x][y - 1][z]) {
					vertex[i - 4] = byte4(x, y + 1, z + 1, side, type);
					vertex[i - 3] = byte4(x, y + 1, z + 1, side, type);
					vertex[i - 1] = byte4(x + 1, y + 1, z + 1, side, type);
					merged++;
				}
				else {
					vertex[i++] = byte4(x, y, z + 1, side, type);
					vertex[i++] = byte4(x + 1, y, z + 1, side, type);
					vertex[i++] = byte4(x, y + 1, z + 1, side, type);
					vertex[i++] = byte4(x, y + 1, z + 1, side, type);
					vertex[i++] = byte4(x + 1, y, z + 1, side, type);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, side, type);
				}
				vis = true;
			}
		}
	}

	changed = false;
	elements = i;

	// If this chunk is empty, no need to allocate a chunk slot.
	if (!elements)
		return;

	// If we don't have an active slot, find one
	if (chunk_slot[slot] != this) {
		int lru = 0;
		for (int i = 0; i < CHUNKSLOTS; i++) {
			// If there is an empty slot, use it
			if (!chunk_slot[i]) {
				lru = i;
				break;
			}
			// Otherwise try to find the least recently used slot
			if (chunk_slot[i]->lastused < chunk_slot[lru]->lastused)
				lru = i;
		}

		// If the slot is empty, create a new VBO
		if (!chunk_slot[lru]) {
			glGenBuffers(1, &vbo);
			// Otherwise, steal it from the previous slot owner
		}
		else {
			vbo = chunk_slot[lru]->vbo;
			chunk_slot[lru]->changed = true;
		}

		slot = lru;
		chunk_slot[slot] = this;
	}

	// Upload vertices

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, i * sizeof *vertex, vertex, GL_STATIC_DRAW);
}

void Chunk::render() {
	if (changed)
		update();

	lastused = now;

	if (!elements)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(attribute_coord);
	glVertexAttribPointer(attribute_coord, 4, GL_BYTE, GL_FALSE, 5 * sizeof(uint8_t), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_BYTE, GL_FALSE, 5 * sizeof(uint8_t), (void*)(4 * sizeof(uint8_t)));
	glEnableVertexAttribArray(attribute_coord);
	glDrawArrays(GL_TRIANGLES, 0, elements);
}
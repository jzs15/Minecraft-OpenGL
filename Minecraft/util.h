#pragma once
#include <glm/glm.hpp>
#include <stb_image.h>
#include <iostream>
#include "filesystem.h"
#include "mesh.h"
#include "config.h"

extern unsigned int grassTexture;
extern Mesh grassMesh;

void init();
Mesh initBlockMesh(int type);
unsigned int loadTexture(char const * path);
unsigned char *getImage(char const *filename, int *x, int *y, int *comp, int req_comp, int value);



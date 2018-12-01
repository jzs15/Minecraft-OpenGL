#pragma once
#include <glm/glm.hpp>
#include <stb_image.h>
#include <iostream>
#include "filesystem.h"
#include "mesh.h"
#include "config.h"
#include "global.h"

extern unsigned int skyTexture;
extern unsigned int grassTexture;
extern Mesh skyMesh;
extern Mesh grassMesh;

void init();
void initWorldMap();
void drawWorld(Shader shader);
Mesh initSkyMesh();
Mesh initBlockMesh(int type);
unsigned int loadTexture(char const * path);
unsigned int loadSkyMap();
unsigned char *getImage(char const *filename, int *x, int *y, int *comp, int req_comp, int value);



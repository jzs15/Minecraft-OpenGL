#pragma once
#include "global.h"
#include "stb_image.h"
#include <iostream>

class Texture
{
public:
	Texture();
	Texture(const char *path);
	~Texture();
	int getWidth();
	int getHeight();
	int getComponents();
	unsigned char* getData();

private:
	int width;
	int height;
	int components;
	unsigned char *data;
};
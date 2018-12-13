#include "texture.h"

Texture::Texture()
{
}

Texture::Texture(const char *path)
{
	data = stbi_load(path, &width, &height, &components, 0);
	if (!data)
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
}

Texture::~Texture()
{
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}

int Texture::getComponents()
{
	return components;
}

unsigned char* Texture::getData()
{
	return data;
}
#include "util.h"
unsigned int grassTexture;
Mesh grassMesh;

void init()
{
	grassTexture = loadTexture(FileSystem::getPath("resources/textures/grass.png").c_str());
	grassMesh = initBlockMesh(GRASS);
}


Mesh initBlockMesh(int type)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int textureID;
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.33333f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.66666f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.6666f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.6666f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.6666f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.3333f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f , 0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.3333f) });
	vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.3333f) });
	vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f) });

	for (unsigned int i = 0; i < 36; i++)
	{
		indices.push_back(i);
	}

	switch (type)
	{
	case GRASS:
		textureID = grassTexture;
		break;
	default:
		break;
	}
	return Mesh(vertices, indices, textureID);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = getImage(path, &width, &height, &nrComponents, 0, 8);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glActiveTexture(GL_TEXTURE0 + textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned char *getImage(char const *filename, int *x, int *y, int *comp, int req_comp, int value)
{
	unsigned char *data = stbi_load(filename, x, y, comp, req_comp);
	int width = *x, height = *y, nrComponents = *comp;
	*x *= value;
	*y *= value;

	int len = width * height * nrComponents;
	unsigned char *enlarged = new unsigned char[len * value * value];
	for (int i = 0; i < height; i++)
	{
		int yIndex = i * width * nrComponents;
		for (int j = 0; j < width; j++)
		{
			int xIndex = j * nrComponents;
			for (int k = 0; k < value; k++)
			{
				memcpy(enlarged + (xIndex * value) + (k * nrComponents) + (yIndex * value * value), data + xIndex + yIndex, nrComponents);
			}
		}

		for (int j = 1; j < value; j++)
		{
			memcpy(enlarged + (yIndex * value * value) + (j * width * nrComponents * value),
				enlarged + (yIndex * value * value), width * nrComponents * value);
		}
	}
	stbi_image_free(data);
	return enlarged;
}
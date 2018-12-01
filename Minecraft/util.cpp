#include "util.h"
unsigned int skyTexture;
unsigned int grassTexture;
Mesh skyMesh;
Mesh grassMesh;
unsigned char ***map;

void init()
{
	initWorldMap();
	skyTexture = loadSkyMap();
	grassTexture = loadTexture(FileSystem::getPath("resources/textures/blocks/grass.png").c_str());

	skyMesh = initSkyMesh();
	grassMesh = initBlockMesh(GRASS);
}

void initWorldMap()
{
	map = new unsigned char**[WORLD_WIDTH];
	for (int i = 0; i < WORLD_WIDTH; i++)
	{
		map[i] = new unsigned char*[WORLD_WIDTH];
		for (int j = 0; j < WORLD_WIDTH; j++)
		{
			map[i][j] = new unsigned char[WORLD_HEIGHT];
			map[i][j][0] = 1;
		}
	}

}

void drawWorld(Shader shader)
{
	shader.use();
	for (int i = 0; i < WORLD_WIDTH; i++)
	{
		for (int j = 0; j < WORLD_WIDTH; j++)
		{
			for (int k = 0; k < WORLD_HEIGHT; k++)
			{
				shader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(i, j, k)));
				switch (map[i][j][k])
				{
				case 1:
					grassMesh.Draw(shader);
					break;
				default:
					break;
				}
			}
		}
	}
}

Mesh initSkyMesh()
{
	return Mesh(skyTexture);
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

unsigned int loadSkyMap()
{
	vector<std::string> faces
	{
		FileSystem::getPath("resources/textures/skybox/right.jpg"),
		FileSystem::getPath("resources/textures/skybox/left.jpg"),
		FileSystem::getPath("resources/textures/skybox/top.jpg"),
		FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
		FileSystem::getPath("resources/textures/skybox/front.jpg"),
		FileSystem::getPath("resources/textures/skybox/back.jpg"),
	};

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
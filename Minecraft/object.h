#pragma once
#include "mesh.h"
#include "shader.h"

class Object
{
public:
	Object();
	Object(Mesh *mesh, glm::vec3 pos = glm::vec3(0.0f));
	~Object();

	void Draw(Shader shader);

public:
	Mesh *mesh;
	glm::vec3 position;
};


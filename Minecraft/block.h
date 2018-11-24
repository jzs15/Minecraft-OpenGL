#pragma once
#include "object.h"

class Block : public Object
{
public:
	Block();
	Block(Mesh *mesh, glm::vec3 pos = glm::vec3(0.0f));
	~Block();
};


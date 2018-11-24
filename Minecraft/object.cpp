#include "object.h"

Object::Object()
{
}


Object::Object(Mesh *mesh, glm::vec3 pos) : mesh(mesh), position(pos)
{
}


Object::~Object()
{
}

void Object::Draw(Shader shader)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	shader.setMat4("model", model);
	mesh->Draw(shader);
}

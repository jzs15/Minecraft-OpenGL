#include "camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 front, float yaw, float pitch)
	: movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	this->position = position;
	this->front = front;
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}


Camera::~Camera()
{
}

void Camera::updateCameraVectors()
{
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, WORLD_UP));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::processKeyboard(unsigned int keys, float deltaTime, World *world)
{
	float velocity = movementSpeed * deltaTime;
	glm::vec3 direction(0.0f);
	if (keys & 1)
	{
		direction -= right;
	}
	if (keys & 2)
	{
		direction += right;
	}
	if (keys & 4)
	{
		direction += front;
	}
	if (keys & 8)
	{
		direction -= front;
	}
	if (glm::length(direction) > 0.5)
	{
		direction = glm::normalize(direction) * velocity;
		setNextPosition(direction, world);
	}
}

void Camera::processMouseMovement(float x, float y)
{
	yaw += x * mouseSensitivity;
	pitch -= y * mouseSensitivity;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getPosition()
{
	return position;
}

glm::vec3 Camera::getFront()
{
	return front;
}

glm::vec3 Camera::getRight()
{
	return right;
}

void Camera::setPosition(glm::vec3 pos)
{
	position = pos;
}


void Camera::setNextPosition(glm::vec3 direction, World *world)
{
	glm::vec3 newPosition = position + direction;
	glm::vec3 floorPosition = glm::floor(position);
	glm::vec3 floorNewPosition = glm::floor(newPosition);
	std::cout << direction.y << std::endl;
	if (direction.x >= 0 && (world->get(floorPosition.x + 1, floorPosition.y, floorPosition.z)
		|| world->get(floorPosition.x + 1, floorPosition.y - 1, floorPosition.z))
		&& (floorPosition.x + 1 - newPosition.x) < 0.25)
	{
		newPosition.x = floorPosition.x + 1 - 0.25;
	}
	else if (direction.x < 0 && (world->get(floorPosition.x - 1, floorPosition.y, floorPosition.z) 
		|| world->get(floorPosition.x - 1, floorPosition.y - 1, floorPosition.z))
		&& (newPosition.x - floorPosition.x) < 0.25)
	{
		newPosition.x = floorPosition.x + 0.25;
	}

	if (direction.y >= 0 && world->get(floorPosition.x, floorPosition.y + 1, floorPosition.z)
		&& (floorPosition.y + 1 - newPosition.y) < 0.1)
	{
		newPosition.y = floorPosition.y + 1 - 0.1;
	}
	else if (direction.y < 0 && world->get(floorPosition.x, floorPosition.y - 2, floorPosition.z)
		&& (newPosition.y - floorPosition.y) < 0.6)
	{
		newPosition.y = floorPosition.y + 0.6;
	}

	if (direction.z >= 0 && (world->get(floorPosition.x, floorPosition.y, floorPosition.z + 1)
		|| world->get(floorPosition.x, floorPosition.y - 1, floorPosition.z + 1))
		&& (floorPosition.z + 1 - newPosition.z) < 0.25)
	{
		newPosition.z = floorPosition.z + 1 - 0.25;
	}
	else if (direction.z < 0 && (world->get(floorPosition.x, floorPosition.y, floorPosition.z - 1)
		|| world->get(floorPosition.x, floorPosition.y - 1, floorPosition.z - 1))
		&& (newPosition.z - floorPosition.z) < 0.25)
	{
		newPosition.z = floorPosition.z + 0.25;
	}

	position = newPosition;
}
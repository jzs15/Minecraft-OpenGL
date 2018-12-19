#include "camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 front, float yaw, float pitch)
	: movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	this->position = position;
	this->front = front;
	this->yaw = yaw;
	this->pitch = pitch;
	this->isWalking = true;
	this->isFalling = false;
	this->isJumping = false;
	this->gravitySpeed = 0.0f;
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

	newFront.x = cos(glm::radians(yaw));
	newFront.y = 0;
	newFront.z = sin(glm::radians(yaw));
	walkFront = glm::normalize(newFront);
	walkRight = glm::normalize(glm::cross(walkFront, WORLD_UP));
}

void Camera::processKeyboard(unsigned int keys, float deltaTime, World *world)
{
	float velocity = movementSpeed * deltaTime;
	glm::vec3 direction(0.0f);
	glm::vec3 curFront = isWalking ? walkFront : front;
	glm::vec3 curRight = isWalking ? walkRight : right;
	if (keys & 1)
	{
		direction -= curRight;
	}
	if (keys & 2)
	{
		direction += curRight;
	}
	if (keys & 4)
	{
		direction += curFront;
	}
	if (keys & 8)
	{
		direction -= curFront;
	}
	if ((keys & 16))
	{
		if (isWalking && !isFalling && !isJumping)
		{
			gravitySpeed = 0.15f;
			isFalling = true;
			isJumping = true;
		}
		else if (!isWalking)
		{
			direction += WORLD_UP;
		}
	}
	if ((keys & 64))
	{
		yaw -= K_MOUSE_VELOCITY;
		updateCameraVectors();
	}
	if ((keys & 128))
	{
		yaw += K_MOUSE_VELOCITY;
		updateCameraVectors();
	}
	if ((keys & 256))
	{
		pitch += K_MOUSE_VELOCITY;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		updateCameraVectors();
	}
	if ((keys & 512))
	{
		pitch -= K_MOUSE_VELOCITY;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		updateCameraVectors();
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

void Camera::gravity(World *world)
{
	if (!isWalking)
	{
		return;
	}
	if (!isFalling)
	{
		glm::vec3 floorPosition = glm::floor(position - glm::vec3(0.0f, 1.4f, 0.0f));
		if (!world->get(floorPosition.x, floorPosition.y, floorPosition.z))
		{
			isFalling = true;
		}
		else
		{
			position.y = floorPosition.y + 2.4f;
		}
	}
	if (isFalling)
	{
		glm::vec3 nextPosition = position - (-WORLD_UP * gravitySpeed);
		glm::vec3 floorNextPosition = glm::floor(nextPosition);
		glm::vec3 floorPosition = glm::floor(position);
		if (gravitySpeed > 0.0f)
		{
			if (world->get(floorPosition.x, floorPosition.y + 1, floorPosition.z)
				&& (floorPosition.y + 1 - nextPosition.y) < 0.1)
			{
				position.y = floorPosition.y + 1 - 0.1;
				isFalling = false;
				gravitySpeed = 0.0f;
			}
		}
		else
		{
			int min_block = nextPosition.y - floorNextPosition.y > 0.4 ? 1 : 2;
			for (int y = floorPosition.y - 1; y >= floorNextPosition.y - min_block; y--)
			{
				if (world->get(floorPosition.x, y, floorPosition.z))
				{
					position.y = (float)y + 2.4f;
					isFalling = false;
					isJumping = false;
					gravitySpeed = 0.0f;
					break;
				}
			}
		}

		if (isFalling)
		{
			position = nextPosition;
			gravitySpeed -= 0.008f;
		}
	}
}

void Camera::changeType()
{
	isWalking = !isWalking;
	gravitySpeed = 0.0f;
}

void Camera::setNextPosition(glm::vec3 direction, World *world)
{
	glm::vec3 newPosition = position + direction;
	glm::vec3 floorPosition = glm::floor(position);
	glm::vec3 floorNewPosition = glm::floor(newPosition);
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
		&& (newPosition.y - floorPosition.y) < 0.4)
	{
		newPosition.y = floorPosition.y + 0.4;
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
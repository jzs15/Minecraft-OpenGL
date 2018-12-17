#pragma once
#include "global.h"
#include "world.h"
#include <iostream>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 7.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 angle = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	~Camera();

	void processKeyboard(unsigned int keys, float deltaTime, World *world);
	void processMouseMovement(float x, float y);
	void updateCameraVectors();
	void setNextPosition(glm::vec3 direction, World *world);
	glm::mat4 getViewMatrix();
	glm::vec3 getPosition();
	glm::vec3 getFront();
	glm::vec3 getRight();
	void gravity(World *world);
	void changeType();



private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 walkFront;
	glm::vec3 walkRight;

	float gravitySpeed;

	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	bool isWalking;
	bool isJumping;
	bool isFalling;
};


#pragma once
/* Credit to https://learnopengl.com/Getting-started/Camera */

#ifndef CAMERA_H
#define CAMERA_H

#include <../glm-0.9.9-a2/glm/glm.hpp>
#include <../glm-0.9.9-a2/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>


// Defines several possible options for camera movement. 
//Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, 
//Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	// Eular Angles
	float yaw;
	float pitch;
	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	// Constructor with vectors
	Camera(glm::vec3 positionIn = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 upIn = glm::vec3(0.0f, 1.0f, 0.0f), 
		float yawIn = YAW, float pitchIn = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), 
		mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		position = positionIn;
		worldUp = upIn;
		yaw = yawIn;
		pitch = pitchIn;
		updateCameraVectors();
	}

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yawIn, float pitchIn) :
		front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		position = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		yaw = yawIn;
		pitch = pitchIn;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}

	// Processes input received from any keyboard-like input system. 
	//Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
			position += front * velocity;
		if (direction == BACKWARD)
			position -= front * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == RIGHT)
			position += right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true)
	{
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;
		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yOffset)
	{
		if (zoom >= 1.0f && zoom <= 45.0f)
			zoom -= yOffset;
		
		//Constrain the zoom to a value between 1 and 45. 
		if (zoom <= 1.0f)
			zoom = 1.0f;
		if (zoom >= 45.0f)
			zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 newFront;
		newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		newFront.y = sin(glm::radians(pitch));
		newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(newFront);
		// Also re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more
			//you look up or down which results in slower movement.
		right = glm::normalize(glm::cross(front, worldUp)); 
		up = glm::normalize(glm::cross(right, front));
	}
};
#endif
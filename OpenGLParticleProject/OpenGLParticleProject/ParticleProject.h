#pragma once

#include "Camera.h"
#include "Shader.h"
#include <string>
#include <array>
#include <sstream>
#include "ParticleSystem.h"

/* Particle Project Class */
/* The home for the project which houses the window and overall system management.
Note that the camera is a global object, since GLFW doesn't take very kindly to member functions.*/
class ParticleSystem;

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const float WINDOW_COLOR_R = 0.1f;
const float WINDOW_COLOR_G = 0.1f;
const float WINDOW_COLOR_B = 0.1f;
const float WINDOW_COLOR_A = 1.0f;
const float CAMERA_SPEED_DEFAULT = 2.5f;
const float CAMERA_FRUSTUM_NEAR = 0.1f;
const float CAMERA_FRUSTUM_FAR = 100.0f;
const glm::vec3 CAMERA_HOME_POSITION = glm::vec3(0.0f, 0.0f, 3.0f);

class ParticleProject
{
private:
	unsigned int windowWidth, windowHeight;

public:
	GLFWwindow* ourWindow;
	Shader ourShader;
	std::vector<ParticleSystem*> sys;
	unsigned int VBO, VAO;
	void processInput(GLFWwindow * window, float deltaTime);

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	
	ParticleProject();
	ParticleProject(unsigned int windowWidth, unsigned int windowHeight);
	int RunProject();
};


void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#pragma once

#include "Camera.h"
#include "Shader.h"
#include <string>
#include <array>
#include <sstream>
/* Particle Class */
/* Particle must contain the data required to function as a particle.
	- Lifetime
	- Acceleration*/
class Particle
{
public:
	float lifetime;
	glm::vec3 particlePosition;
	glm::vec3 movementVector;
	//Let's try and get a particle to appear and disappear to start.
	//const glm::vec3 acceleration;
	//glm::vec3 velocity;
	Particle();
	Particle(glm::vec3 _origin, float _lifetime);
	void setMovementVector(glm::vec3 _movementVector);
	glm::vec3 ParticleUpdate(float deltaTime);
};

/* Particle System Class */
/* Particle System needs to manage the array of particles it produces.
	- We need to be regularly updating the particles. 
	- We need to keep track of which ones are destroyed and replacing them when they are destroyed. 
	- We need to keep the shape of the particles as an array of vertices. 
It also should be movable itself to provide different effects, or at the very least
help to build up the motion for the particles themselves.
	- The system needs it's own position to act as an origin for the particles. 
	- The system has it's own update function so it can also move if it needs too.*/
class ParticleSystem
{
private:
	bool willTrail;
public:
	bool isOn = false;
	//Position of the particle system itself. Origin for all particles. 
	glm::vec3 systemOrigin = glm::vec3(0.0f, -10.0f, -40.0f);
	glm::vec3 systemPosition = systemOrigin;
	glm::vec3 systemMovementVector = glm::vec3 (0.0f, 0.0f, 0.0f);
	glm::vec3 systemInitialVelocity;
	glm::vec3 gravity = glm::vec3(0.0f, -0.005f, 0.0f);
	glm::vec3 colorVector = glm::vec3(1.0f, 1.0f, 1.0f);
	Particle* particleArray[100];
	//Vertices to make a cube for visualizing systemPosition.
	float vertices[108] = {
		-0.05f, -0.05f, -0.05f,
		0.05f, -0.05f, -0.05f,
		0.05f,  0.05f, -0.05f,
		0.05f,  0.05f, -0.05f,
		-0.05f,  0.05f, -0.05f,
		-0.05f, -0.05f, -0.05f,

		-0.05f, -0.05f,  0.05f,
		0.05f, -0.05f,  0.05f,
		0.05f,  0.05f,  0.05f,
		0.05f,  0.05f,  0.05f,
		-0.05f,  0.05f,  0.05f,
		-0.05f, -0.05f,  0.05f,

		-0.05f,  0.05f,  0.05f,
		-0.05f,  0.05f, -0.05f,
		-0.05f, -0.05f, -0.05f,
		-0.05f, -0.05f, -0.05f,
		-0.05f, -0.05f,  0.05f,
		-0.05f,  0.05f,  0.05f,

		0.05f,  0.05f,  0.05f, 
		0.05f,  0.05f, -0.05f, 
		0.05f, -0.05f, -0.05f, 
		0.05f, -0.05f, -0.05f, 
		0.05f, -0.05f,  0.05f, 
		0.05f,  0.05f,  0.05f, 

		-0.05f, -0.05f, -0.05f,
		0.05f, -0.05f, -0.05f, 
		0.05f, -0.05f,  0.05f, 
		0.05f, -0.05f,  0.05f, 
		-0.05f, -0.05f,  0.05f,
		-0.05f, -0.05f, -0.05f,

		-0.05f,  0.05f, -0.05f,
		0.05f,  0.05f, -0.05f, 
		0.05f,  0.05f,  0.05f, 
		0.05f,  0.05f,  0.05f, 
		-0.05f,  0.05f,  0.05f,
		-0.05f,  0.05f, -0.05f,
	};
	std::array<float, 3> pSystemPhases; //Phases to keep track of changes throughout the particle's lifetime.
	

	ParticleSystem();
	ParticleSystem(glm::vec3 launchVector);
	void ParticleSystemUpdate(float _deltaTime);
	//void SetSystemOrigin(glm::vec3 newOrigin);
	void SetSystemMovementVector(glm::vec3 newVector);
	//SetSystemColorVector
	//
	void Pop();
	glm::vec3 RandomizeParticleVector(int type, float rotationArray[]);
	void Reset();
	
	//isOn defaults to true
	void ToggleSystem() {
		if (isOn) isOn = false;
		else isOn = true;
	};
};

/* Particle Project Class */
/* The home for the project which houses the window and overall system management.
Note that the camera is a global object, since GLFW doesn't take very kindly to member functions.*/
class ParticleProject
{
private:
	unsigned int ScreenWidth, ScreenHeight;

public:
	GLFWwindow* ourWindow;
	Shader ourShader;
	//Camera ourCamera;
	std::vector<ParticleSystem*> sys;
	unsigned int VBO, VAO, EBO;
	void processInput(GLFWwindow * window, float deltaTime);

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	
	ParticleProject();
	ParticleProject(unsigned int ScreenWidth, unsigned int ScreenHeight);
	int RunProject();
};


void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

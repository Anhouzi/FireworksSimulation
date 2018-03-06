#pragma once
#include "Particle.h"
#include "ParticleProject.h"

/* Particle System Class */
/* Particle System needs to manage the array of particles it produces.
- We need to be regularly updating the particles.
- We need to keep track of which ones are destroyed and replacing them when they are destroyed.
- We need to keep the shape of the particles as an array of vertices.
It also should be movable itself to provide different effects, or at the very least
help to build up the motion for the particles themselves.
- The system needs it's own position to act as an origin for the particles.
- The system has it's own update function so it can also move if it needs too.*/

class Particle;

//Particle System Variable Constants
const int CUBE_VERTICES = 108;
const int SQUARE_VERTICES = 4;
const int PARTICLE_ARRAY_SIZE_MAX = 100;
const int PARTICLE_SYSTEM_EXPLOSION_TYPECOUNT = 2;
const float PARTICLE_SIZE_DIVIDEND = 20.0f; //Divides the particle size by this number.
const float LIFESPAN_DEFAULT = 2.0f;


//Color Vector Constants
const int COLOR_ATTRIBUTE_MIN = 20;
const int COLOR_ATTRIBUTE_RAND = 100 - COLOR_ATTRIBUTE_MIN;

//Movement Vector Constants
const float GRAVITY = -0.005f; //The rate at which particles will fall. 
const float VELOCITY_REDUCTION_CONSTANT = 3.0f; //Divides normalized velocities by this value to slow them down.


//Vector Constants
const glm::vec3 PARTICLE_SYSTEM_ORIGIN_DEFAULT = glm::vec3(0.0f, -10.0f, -40.0f);
const glm::vec3 PARTICLE_SYSTEM_MOVEMENT_DEFAULT = glm::vec3(0.0f, 0.25f, 0.0f);

class ParticleSystem
{
private:
	bool willTrail;
public:
	bool isOn = false;
	//Position of the particle system itself. Origin for all particles. 
	glm::vec3 systemOrigin = PARTICLE_SYSTEM_ORIGIN_DEFAULT;
	glm::vec3 systemPosition = systemOrigin;
	glm::vec3 systemMovementVector = glm::vec3(0.0f, 0.0f, 0.0f); //Initialized to be still. 
	glm::vec3 systemInitialVelocity;
	glm::vec3 gravity = glm::vec3(0.0f, GRAVITY, 0.0f);
	glm::vec3 colorVector = glm::vec3(1.0f, 1.0f, 1.0f); //Initialized to white.
	Particle* particleArray[PARTICLE_ARRAY_SIZE_MAX];
	//Vertices to make a cube for visualizing systemPosition.
	float vertices[CUBE_VERTICES] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
	};

	//Constructor and Constructor related functions
	ParticleSystem();
	ParticleSystem(glm::vec3 launchVector);
	glm::vec3 RandomizeColorVector();
	void ParticleSystemSetup();


	void ParticleSystemUpdate(float _deltaTime);
	void SetSystemMovementVector(glm::vec3 newVector);

	void Pop();
	glm::vec3 RandomizeParticleVector(int type, float rotationArray[]);
	void Reset();

	//isOn defaults to true
	void ToggleSystem() {
		if (isOn) isOn = false;
		else isOn = true;
	};
};
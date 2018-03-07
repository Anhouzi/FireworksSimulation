#include "Particle.h"
//=======================================
/* PARTICLE CLASS CODE */
//=======================================
//Base constructor that just uses the other constructor with given values. 
Particle::Particle()
{
	Particle(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
}

//Particle constructor - Creates a random movement vector that the particle will follow.
Particle::Particle(glm::vec3 _origin, float _lifetime)
{
	lifetime = _lifetime;
	particlePosition = _origin;
	SetMovementVector(glm::vec3(0.0f, 0.0f, 0.0f)); //Initialize to be still.
}

//We get a directional vector than adjust the vector's magnitude before giving it to the particle. 
void Particle::SetMovementVector(glm::vec3 inputVector)
{
	glm::vec3 vec;
	//Higher values mean smaller radii for explosions.
	float particleVelocityDividend = 5.0f;

	//By normalizing the vector, we retain the direction of the vector, but limit the speed to whatever magnitude we define. 	
	vec = glm::normalize(inputVector) / particleVelocityDividend;
	movementVector = vec;
}

//Update function for the particle, we return the particle's position at the end.
glm::vec3 Particle::ParticleUpdate(float deltaTime)
{
	lifetime -= deltaTime;
	//movementVector.y += GRAVITY; //Simulation for gravity.
	particlePosition += movementVector;
	return particlePosition;
}
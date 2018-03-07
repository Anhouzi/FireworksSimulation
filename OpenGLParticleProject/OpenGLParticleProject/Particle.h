#pragma once
#include "ParticleProject.h"

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
	Particle();
	Particle(glm::vec3 _origin, float _lifetime);
	void SetMovementVector(glm::vec3 _movementVector);
	glm::vec3 ParticleUpdate(float deltaTime);
};

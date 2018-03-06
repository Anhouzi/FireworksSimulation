#include "ParticleSystem.h"

//=======================================
/* PARTICLE SYSTEM CLASS CODE */
//=======================================
//Base Particle System constructor. Uses a fixed movement vector instead of a passed parameter.
ParticleSystem::ParticleSystem()
{
	SetSystemMovementVector(PARTICLE_SYSTEM_MOVEMENT_DEFAULT);
	colorVector = RandomizeColorVector();
	ParticleSystemSetup();
}

ParticleSystem::ParticleSystem(glm::vec3 launchVector)
{
	glm::vec3 newMovementVector = glm::normalize(launchVector) / VELOCITY_REDUCTION_CONSTANT;
	SetSystemMovementVector(newMovementVector);
	colorVector = RandomizeColorVector();
	ParticleSystemSetup();
}

//Creates and returns a random vec3 to act as the color for each particle system. 
glm::vec3 ParticleSystem::RandomizeColorVector()
{
	float colorVectorNormalizeValue = 100; //Brings the color vector down to a value between 0 and 1.
	glm::vec3 newColor = glm::vec3(
		(float)(rand() % COLOR_ATTRIBUTE_RAND + COLOR_ATTRIBUTE_MIN) / colorVectorNormalizeValue,
		(float)(rand() % COLOR_ATTRIBUTE_RAND + COLOR_ATTRIBUTE_MIN) / colorVectorNormalizeValue,
		(float)(rand() % COLOR_ATTRIBUTE_RAND + COLOR_ATTRIBUTE_MIN) / colorVectorNormalizeValue);
	return newColor;
}

void ParticleSystem::ParticleSystemSetup()
{
	//Scale the particle sizes by the dividend constant.
	for (int i = 0; i < CUBE_VERTICES; ++i)
	{
		vertices[i] /= PARTICLE_SIZE_DIVIDEND;
	}

	//Initialize all of the particles in the array.
	for (int i = 0; i < PARTICLE_ARRAY_SIZE_MAX; ++i)
	{
		particleArray[i] = nullptr;
	}
}

//Update function for the rocket as a whole. Takes in the time between each frame so each particle can update as well.
void ParticleSystem::ParticleSystemUpdate(float _deltaTime)
{
	//Boolean to confirm that the particles have expended their lifetime.
	bool isDone = false;

	//If we are not generating particles.
	if (!isOn)
	{
		//Once the rocket stops rising we trigger an explosion.
		if (systemMovementVector.y <= 0)
		{
			isOn = true;
			Pop();
		}
		//If the rocket continues rising, we need to change the rocket's trajectory.
		else
		{
			systemPosition += systemMovementVector;
			systemMovementVector += gravity;
		}
	}

	//If we are generating particles.
	if (isOn)
	{
		//Loop through the particle array.
		for (int i = 0; i < (sizeof(particleArray) / sizeof(*particleArray)); ++i)
		{
			if (particleArray[i] != nullptr)
			{
				//Update each particle.
				particleArray[i]->ParticleUpdate(_deltaTime);

				//Once the particles have no lifespan remaining, the system has been completed and we can reset.
				if (particleArray[i]->lifetime < 0)
				{
					isDone = true;
				}
			}
		}
	}

	if (isDone)
	{
		Reset();
	}
}

void ParticleSystem::SetSystemMovementVector(glm::vec3 newVector)
{
	systemMovementVector = systemInitialVelocity = newVector;
}

//Explosion function that randomizes the type and orientation explosion.
void ParticleSystem::Pop()
{
	const int randomAngleCap = 360;
	int type = rand() % PARTICLE_SYSTEM_EXPLOSION_TYPECOUNT;
	float explosionAngles[3] = { 0.0f, 0.0f, 0.0f };

	//Randomizes angles for explosions.
	if (type > 0)
	{
		explosionAngles[0] = (float)(rand() % randomAngleCap);
		explosionAngles[1] = (float)(rand() % randomAngleCap);
		explosionAngles[2] = (float)(rand() % randomAngleCap);
	}

	//For each particle
	for (int i = 0; i < PARTICLE_ARRAY_SIZE_MAX; ++i)
	{
		//Randomize each particle vector. 
		glm::vec3 vec = RandomizeParticleVector(type, explosionAngles);
		
		if (particleArray[i] != nullptr)
		{
			//Change the particles attributes to be the rocket's position at the time of the explosion. 
			particleArray[i]->particlePosition = systemPosition;
			particleArray[i]->lifetime = LIFESPAN_DEFAULT;
			particleArray[i]->setMovementVector(vec);
		}
		//If the particles haven't been created yet, we need to make them. 
		else
		{
			particleArray[i] = new Particle(systemPosition, LIFESPAN_DEFAULT);
			particleArray[i]->setMovementVector(vec);
		}

	}
}

//Create the directional vector for each particle. 
glm::vec3 ParticleSystem::RandomizeParticleVector(int type, float rotationArray[])
{
	const int streamCount = 7;
	float varianceX;
	int xAngleRand = 200;
	int xAngleCenter = -xAngleRand / 2;

	float varianceY;
	int yVelocity = 150;
	int yVelocityCenter = -yVelocity / 2;

	float varianceZ;
	int zAngleRand = 200;
	int zAngleCenter = -zAngleRand / 2;

	switch (type)
	{
	case 0: // Type 0
		 //Sphere
		varianceX = ((float)(rand() % xAngleRand + xAngleCenter));
		varianceY = ((float)(rand() % yVelocity + yVelocityCenter));
		varianceZ = ((float)(rand() % zAngleRand + zAngleCenter));
		break;
	case 1:
		// Hemisphere
		varianceX = ((float)(rand() % xAngleRand + xAngleCenter));
		varianceY = ((float)(rand() % yVelocity));
		varianceZ = ((float)(rand() % zAngleRand + zAngleCenter));
		break;

		//More Explosion types would go here. 

	default:
		break;
	}

	//Must be a single value for each system, or else particles go pretty much everywhere.
	glm::vec4 malleableVector = glm::normalize(glm::vec4(varianceX, varianceY, varianceZ, 1.0f));
	glm::mat4 xRotation = glm::mat3(1.0f);
	glm::mat4 yRotation = glm::mat3(1.0f); //An unneccesary rotation with the project's current POV. 
	glm::mat4 zRotation = glm::mat3(1.0f);

	//Rotate each axis based on the rotationArray parameter. 
	xRotation = glm::rotate(xRotation, glm::radians(rotationArray[0]), glm::vec3(1, 0, 0));
	yRotation = glm::rotate(yRotation, glm::radians(rotationArray[1]), glm::vec3(0, 1, 0));
	zRotation = glm::rotate(zRotation, glm::radians(rotationArray[2]), glm::vec3(0, 0, 1));

	//Modify the malleable vector. 
	malleableVector = zRotation * yRotation * xRotation * malleableVector;

	//Gather the directional compontents and return the new vec3. 
	glm::vec3 vec = glm::vec3(malleableVector.x, malleableVector.y, malleableVector.z);
	return vec;
}

//Resets the rocket to it's origins and refires it.
void ParticleSystem::Reset()
{
	systemPosition = systemOrigin;
	systemMovementVector = systemInitialVelocity;
	isOn = false;
}


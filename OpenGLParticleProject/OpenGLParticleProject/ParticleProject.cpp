#include "ParticleProject.h"

//Camera/controller related variables
float lastX = 400;
float lastY = 300; //Center of the screen and also the starting mouse position.

// mouse directions
bool firstMouse = true;
bool isPressed = false;

Camera ourCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
	setMovementVector(glm::vec3(1.0f, 1.0f, 1.0f));
}

//We get a directional vector than adjust the vector's magnitude before giving it to the particle. 
void Particle::setMovementVector(glm::vec3 _movementVector)
{
	glm::vec3 vec; 
	//Higher values mean smaller radii for explosions.
	float particleVelocityDividend = 5.0f; 

	//By normalizing the vector, we retain the direction of the vector, but limit the speed to whatever magnitude we define. 	
	vec = glm::normalize(_movementVector) / particleVelocityDividend;
	movementVector = vec;
}

//Update function for the particle, we return the particle's position at the end.
glm::vec3 Particle::ParticleUpdate(float deltaTime)
{
	lifetime -= deltaTime;
	movementVector.y -= 0.0005f; //Simulation for gravity.
	particlePosition += movementVector;
	return particlePosition;
}

//=======================================
/* PARTICLE SYSTEM CLASS CODE */
//=======================================
//Base Particle System constructor. Uses a fixed movement vector instead of a passed parameter.
ParticleSystem::ParticleSystem()
{
	SetSystemMovementVector(glm::vec3(0.0f, 0.25f, 0.0f));
	//Randomizes the color of the particle system.
	colorVector = glm::vec3(
		(float)(rand() % 80 + 20) / 100,
		(float)(rand() % 80 + 20) / 100,
		(float)(rand() % 80 + 20) / 100);

	//Initialize all of the particles in the array.
	for (int i = 0; i < 100; ++i)
	{
		particleArray[i] = nullptr;
	}
}

ParticleSystem::ParticleSystem(glm::vec3 launchVector)
{
	SetSystemMovementVector(launchVector);
	colorVector = glm::vec3((float)(rand() % 80 + 20) / 100, 
							(float)(rand() % 80 + 20) / 100, 
							(float)(rand() % 80 + 20) / 100);
	for (int i = 0; i < 100; ++i)
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
	int type = rand() % 3;
	float explosionAngles[3] = {0.0f, 0.0f, 0.0f};

	//Randomizes angles for explosions.
	if (type > 0)
	{
		explosionAngles[0] = (float)(rand() % 360);
		explosionAngles[1] = (float)(rand() % 360);
		explosionAngles[2] = (float)(rand() % 360);
	}

	//For each particle
	for (int i = 0; i < 100; ++i)
	{
		//Randomize each particle vector. 
		glm::vec3 vec = RandomizeParticleVector(type, explosionAngles);


		float life = 2.0f;
		if (particleArray[i] != nullptr)
		{
			//Change the particles attributes to be the rocket's position at the time of the explosion. 
			particleArray[i]->particlePosition = systemPosition;
			particleArray[i]->lifetime = life;
			particleArray[i]->setMovementVector(vec);
		}
		//If the particles haven't been created yet, we need to make them. 
		else
		{
			particleArray[i] = new Particle(systemPosition, life);
			particleArray[i]->setMovementVector(vec);
		}
		
	}
}

//Create the directional vector for each particle. 
glm::vec3 ParticleSystem::RandomizeParticleVector(int type, float rotationArray[])
{
	const int streamCount = 7;
	float varianceX;
	float varianceY;
	float varianceZ;

	
	switch (type)
	{
	case 1:
			// Hemisphere
		varianceX = ((float)(rand() % 200 - 100) / 5000.0f);
		varianceY = ((float)(rand() % 100) / 5000.0f);
		varianceZ = ((float)(rand() % 200 - 100) / 5000.0f);
		break;

		/* Maintains the shape but looks awkward because it's not obeying physics yet.
	case 2: 
			// Cone/Pyramid
		std::cout << "Pyramid" << std::endl;
		varianceX = ((float)(rand() % 20 - 10) / 5000.0f);
		varianceY = ((float)(rand() % 20 + 80) / 5000.0f);
		varianceZ = ((float)(rand() % 20 - 10) / 5000.0f);
		break;
		*/

	case 3:
		// Streamed Sphere
		glm::vec3 streams[streamCount];
		for (int i = 0; i < streamCount; i++)
		{
			float varianceX = ((float)(rand() % 200 - 100) / 5000.0f);
			float varianceY = ((float)(rand() % 200 - 100) / 5000.0f);
			float varianceZ = ((float)(rand() % 200 - 100) / 5000.0f);

			streams[i] = glm::vec3(varianceX, varianceY, varianceZ);
		}
		
	default: // Type-0
		//Sphere
		//std::cout << "Sphere" << std::endl;

		varianceX = ((float)(rand() % 200 - 100) / 5000.0f);
		varianceY = ((float)(rand() % 200 - 100) / 5000.0f);
		varianceZ = ((float)(rand() % 200 - 100) / 5000.0f);
		break;
	}
	
	//Must be a single value for each system, or else particles go pretty much everywhere.
	//float testRotationAngle = 0; //Degrees
	glm::vec4 malleableVector = glm::vec4(varianceX, varianceY, varianceZ, 1.0f);
	glm::mat4 xRotation = glm::mat3(1.0f);
	glm::mat4 yRotation = glm::mat3(1.0f); //An unneccesary rotation with the project's current POV. 
	glm::mat4 zRotation = glm::mat3(1.0f);

	xRotation = glm::rotate(xRotation, glm::radians(rotationArray[0]), glm::vec3(1, 0, 0));
	yRotation = glm::rotate(yRotation, glm::radians(rotationArray[1]), glm::vec3(0, 1, 0));
	zRotation = glm::rotate(zRotation, glm::radians(rotationArray[2]), glm::vec3(0, 0, 1));

	malleableVector = zRotation * yRotation * xRotation * malleableVector;


	glm::vec3 vec = glm::vec3(malleableVector.x, malleableVector.y, malleableVector.z);
	//glm::vec3 vec = glm::vec3(varianceX, varianceY, varianceZ);
	return vec;
}

//Resets the rocket to it's origins and refires it.
void ParticleSystem::Reset()
{
	systemPosition = systemOrigin;
	systemMovementVector = systemInitialVelocity;
	isOn = false;
}


//=======================================
/* PARTICLE PROJECT CLASS CODE */
//=======================================

ParticleProject::ParticleProject()
{
	ParticleProject(800, 600);
}

//Constructor for the project. Sets up most of the resources for the rest of the program including the window itself.
ParticleProject::ParticleProject(unsigned int _ScreenWidth, unsigned int _ScreenHeight)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	
	// --------------------
	ScreenWidth = _ScreenWidth;
	ScreenHeight = _ScreenHeight;
	ourWindow = glfwCreateWindow(ScreenWidth, ScreenHeight, "Particle Project", NULL, NULL);
	if (ourWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(ourWindow);
	glfwSetFramebufferSizeCallback(ourWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(ourWindow, mouse_callback);
	glfwSetScrollCallback(ourWindow, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(ourWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glEnable(GL_DEPTH_TEST);

	//Allows us to use blends of alpha values. Allowing us have non-integer opacity values for anything we draw.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ourShader = Shader ("Version0.vs", "Version0.fs"); // you can name your shader files however you like
	
	ParticleSystem* testSys = new ParticleSystem();
	
	sys.push_back(testSys);

	glGenVertexArrays(1, &VAO); //Since we are only creating one shape, we shouldn't need more vertex arrays.
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sys[0]->vertices), sys[0]->vertices, GL_STREAM_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sys->indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	ourShader.use();
	return;
}

// Runs the project in the created window. The project will run until the window is closed with the ESC key.
int ParticleProject::RunProject()
{
	glm::vec3 translations[100];
	int index = 0;
	float offset = 0.1f;
	//Can change this to a normal for loop now since we only need one origin. 
	/*
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			glm::vec3 translation;
			//translation.x = (float)x / 10.0f + offset;
			//translation.y = (float)y / 10.0f + offset;
			//translation.z = (float)z / 10.0f + offset;
			//translation.z = 0.0f;
			//translations[index++] = translation;
			translation = sys->systemPosition;
			sys->particleArray[index++]->particlePosition = translation;
		}
	}
	*/
	while (!glfwWindowShouldClose(ourWindow))
	{
		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// calculate frame timings 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(ourWindow, deltaTime);

		//Update all particle systems.
		for (int i = 0; i < sys.size(); ++i)
		{
			sys[i]->ParticleSystemUpdate(deltaTime);
		}
		

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		ourShader.use();		

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(ourCamera.Zoom), (float)ScreenWidth / (float)ScreenHeight, 
								0.1f, 100.0f);
		//std::cout << "Projection Matrix: " << projection[0][0] << " " << projection[1][1] << " " << projection[2][2] << std::endl;
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = ourCamera.GetViewMatrix();
		//std::cout << "View Matrix: " << view[0][0] << " " << view[1][1] << " " << view[2][2] << std::endl;
		ourShader.setMat4("view", view);
		//std::cout << std::endl << "=================" << std::endl;
		glm::mat4 model = glm::mat4(1.0f);
		//std::cout << "Model Matrix: " << model[0][0] << model[1][1] << model[2][2] << std::endl;
		
	
		//Loop through all existing particle systems and run them.
		for (int i = 0; i < sys.size(); ++i)
		{
			//If the rocket hasn't exploded, we will draw the rocket
			if (!sys[i]->isOn)
			{
				//Modifying the vertex shader with the position of every rocket.
				ourShader.setVec3("offsets[0]", sys[i]->systemPosition);
				ourShader.setVec3("pColor", sys[i]->colorVector);
				//model = glm::translate(model, sys->systemPosition);
				//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
				ourShader.setFloat("lifetime", 1.0f);
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			//If the rocket has exploded, we must draw all of the particles in the rocket's particle array. 
			if (sys[i]->isOn)
			{
				glm::mat4 model = glm::mat4(1.0f);
				//std::cout << "Model Matrix: " << model[0][0] << model[1][1] << model[2][2] << std::endl;
				ourShader.setMat4("model", model);
				for (unsigned int j = 0; j < 100; j++)
				{
					//The string stream allows us to dynamically update the vertex shader's offset array for each particle.
					std::stringstream ss;
					std::string index;
					ss << j;
					index = ss.str();
					ourShader.setVec3(("offsets[" + index + "]").c_str(), sys[i]->particleArray[j]->particlePosition);
					ourShader.setVec3("pColor", sys[i]->colorVector);
					ourShader.setFloat("lifetime", sys[i]->particleArray[j]->lifetime);
					//We could set the color of the particles here and remove the color coordinates from the verticies list.

				}
				//Currently, it seems like we can draw as many things as we have offsets in the vertex shader's offsets array. 
					//Also, performance slows down at around 10+ rockets due to either too many draw calls or too many update calls
					//for each particle and particle system.
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
			}
		}
		
		glfwSwapBuffers(ourWindow);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 1;
}

/*Utility Functions*/

void ParticleProject::processInput(GLFWwindow *window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		//cameraPos += cameraSpeed * cameraFront;
		ourCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		//cameraPos -= cameraSpeed * cameraFront;
		ourCamera.ProcessKeyboard(BACKWARD, deltaTime);

	//We take the cross product of the Forward and Up vectors to get the right vector, then use that vector
	// to modify the camera's position.
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		ourCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		ourCamera.ProcessKeyboard(RIGHT, deltaTime);

	//When the user presses Q we will create another particle system and fire another rocket.
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		//Make a semi-random trajectory for the new rocket.
		float x, y, z;
		x = (float)(rand() % 120 - 60) / 200.0f;
		y = (float)(rand() % 60 + 40) / 200.0f;
		z = -(float)(rand() % 60 + 40) / 200.0f;

		if (!isPressed)
		{
			sys.push_back(new ParticleSystem(glm::vec3(x, y, z)));
			isPressed = true;
		}
	}
	//Conditional statement to ensure we aren't continuously firing rockets while the button is held.
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
	{
		if (isPressed)
		{
			isPressed = false;
		}
	}		
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	ourCamera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ourCamera.ProcessMouseScroll(yoffset);
}
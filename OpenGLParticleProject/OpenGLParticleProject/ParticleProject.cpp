#include "ParticleProject.h"

//Camera/controller related variables
float lastX = 400;
float lastY = 300; //Center of the screen and also the starting mouse position.
bool firstMouse = true;
bool isPressed = false;

Camera ourCamera = Camera(CAMERA_HOME_POSITION);

//=======================================
/* PARTICLE PROJECT CLASS CODE */
//=======================================

ParticleProject::ParticleProject()
{
	ParticleProject(WINDOW_WIDTH, WINDOW_HEIGHT);
}

//Constructor for the project. Sets up most of the resources for the rest of the program including the window itself.
ParticleProject::ParticleProject(unsigned int widthIn, unsigned int heightIn)
{
	// Initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //uncomment this statement to fix compilation on OS X
	#endif

	// Window creation
	windowWidth = widthIn;
	windowHeight = heightIn;
	ourWindow = glfwCreateWindow(windowWidth, windowHeight, "Particle Project", NULL, NULL);
	if (ourWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(ourWindow);
	glfwSetFramebufferSizeCallback(ourWindow, FramebufferSizeCallback);
	glfwSetCursorPosCallback(ourWindow, MouseCallback);
	glfwSetScrollCallback(ourWindow, ScrollCallback);

	//Tell GLFW to capture the mouse
	glfwSetInputMode(ourWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	//Allows us to use blends of alpha values. Allowing us have non-integer opacity values for anything we draw.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ourShader = Shader ("../Resources/Version0.vs", "../Resources/Version0.fs"); 
	
	ParticleSystem* testSys = new ParticleSystem();
	
	sys.push_back(testSys);

	glGenVertexArrays(1, &VAO); //Since we are only creating one shape, we shouldn't need more vertex arrays.
	glGenBuffers(1, &VBO);

	//Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sys[0]->vertices), sys[0]->vertices, GL_STREAM_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	ourShader.use();
	return;
}

// Runs the project in the created window. The project will run until the window is closed with the ESC key.
int ParticleProject::RunProject()
{

	while (!glfwWindowShouldClose(ourWindow))
	{
		// Render
		glClearColor(WINDOW_COLOR_R, WINDOW_COLOR_G, WINDOW_COLOR_B, WINDOW_COLOR_A);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate frame timings 
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(ourWindow, deltaTime);

		//Update all particle systems.
		for (unsigned int i = 0; i < sys.size(); ++i)
		{
			sys[i]->ParticleSystemUpdate(deltaTime);
		}
	
		glBindVertexArray(VAO);
		
		ourShader.use();		

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(ourCamera.zoom), (float)windowWidth / (float)windowHeight,
			CAMERA_FRUSTUM_NEAR, CAMERA_FRUSTUM_FAR);
		ourShader.setMat4("projection", projection);
		glm::mat4 view = ourCamera.GetViewMatrix();
		ourShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
	
		//Loop through all existing particle systems and run them.
		for (unsigned int i = 0; i < sys.size(); ++i)
		{
			//If the rocket hasn't exploded, we will draw the rocket
			if (!sys[i]->isOn)
			{
				//Modifying the vertex shader with the position of every rocket.
				ourShader.setVec3("offsets[0]", sys[i]->systemPosition);
				ourShader.setVec3("pColor", sys[i]->colorVector);
				ourShader.setFloat("lifetime", LIFESPAN_DEFAULT);
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			//If the rocket has exploded, we must draw all of the particles in the rocket's particle array. 
			if (sys[i]->isOn)
			{
				glm::mat4 model = glm::mat4(1.0f);
				ourShader.setMat4("model", model);
				for (unsigned int j = 0; j < PARTICLE_ARRAY_SIZE_MAX; j++)
				{
					//The string stream allows us to dynamically update the vertex shader's offset array for each particle.
					std::stringstream stringStream;
					std::string index;
					stringStream << j;
					index = stringStream.str();
					ourShader.setVec3(("offsets[" + index + "]").c_str(), sys[i]->particleArray[j]->particlePosition);
					ourShader.setVec3("pColor", sys[i]->colorVector);
					ourShader.setFloat("lifetime", sys[i]->particleArray[j]->lifetime);
					//We could set the color of the particles here and remove the color coordinates from the verticies list.

				}
				//Currently, it seems like we can draw as many things as we have offsets in the vertex shader's offsets array. 
					//Also, performance slows down at around 10+ rockets due to either too many draw calls or too many update calls
					//for each particle and particle system.
				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100);
			}
		}
		
		glfwSwapBuffers(ourWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 1;
}

/*Utility Functions*/

void ParticleProject::processInput(GLFWwindow *window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = CAMERA_SPEED_DEFAULT * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(BACKWARD, deltaTime);

	//We take the cross product of the Forward and Up vectors to get the right vector, then use that vector
	// to modify the camera's position.
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(RIGHT, deltaTime);

	//When the user presses Q we will create another particle system and fire another rocket.
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		//Make a semi-random trajectory for the new rocket.
		float x, y, z;
		int xAngleRange = 120; 
		int xCenterAngle = -xAngleRange / 2;

		int yVelocityMin = 40; 
		int yVelocityRand = 100 - yVelocityMin;

		int zAngleRange = 60;
		int zCenterAngle = -zAngleRange / 2;



		x = (float)(rand() % xAngleRange + xCenterAngle);
		y = (float)(rand() % yVelocityRand + yVelocityMin);
		z = (float)(rand() % zAngleRange + zCenterAngle);

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

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xOffset = (float)xpos - lastX;
	float yOffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;

	ourCamera.ProcessMouseMovement(xOffset, yOffset); 
}

// Whenever the mouse scroll wheel scrolls, this callback is called
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ourCamera.ProcessMouseScroll((float)yoffset);
}
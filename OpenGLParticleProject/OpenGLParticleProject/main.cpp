//Main
#include "ParticleProject.h"
#include <time.h>

/* Project Documentation */
/* "Fireworks Simulation / Particle System Project" by Matthew Freeman */

/* Project Background: 
	This is a project I tasked myself with after completing the first portion of a tutorial on OpenGL.
	- Link: https://learnopengl.com/Getting-started/
	Through this project, I intend to practice and further my education with Linear Algebra, C++ programming,
	and Graphics Programming using OpenGL by adapting the lessons in this tutorial to make a dynamic fireworks show
	for the user to enjoy.*/

/* Project Post-Mortem: 
	While I full acknowledge that there is plenty more for me to do here, I currently lack the experience to add 
	more advanced features in a reasonable time frame. A few of these features include:
		- Trails that rockets and individual particles might leave behind. 
		- Better application performance in the form of either fewer draw calls or update functions. 
		- More types of explosions for a more varied display. 

	Some things my future self should look into changing within the current project would be:
	1. To refactor the relationship between Particle Systems and Particles
		- After reviewing how particle systems work in the Unreal Engine, there are many aspects of the Particle class
		that could simply be inherited from the Particle System class rather than storing that data in two different places.
	2. To look into more effectively utilizing shaders for drawing
		- After some reading it seems that sending data to shaders is a performance bottleneck and with the huge array we have
		in the vertex shader that is likely a large cause of the performance slowdown when we are at more than 10 rockets.
	3. Find a better way to interface with the camera or remove the camera entirely
		- Due to some restrictions with OpenGL and member functions, some of the variables required to operate the camera 
		were forced to be global variables. While it's fine for a private project like this, it's common knowledge that 
		global variables are not safe to use in larger applications. */

class ParticleProject;

void main()
{
	srand(time(NULL));
	ParticleProject* project = new ParticleProject(WINDOW_WIDTH, WINDOW_HEIGHT);
	project->RunProject();
	return;
}

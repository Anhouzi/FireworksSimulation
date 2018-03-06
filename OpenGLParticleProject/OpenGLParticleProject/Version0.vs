#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform float lifetime; 
uniform vec3 offsets[1000];
uniform vec3 pColor;

//Camera uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	//Local variables. 
	vec3 offset = offsets[gl_InstanceID];
	vec3 vsColor = pColor;

	//Adds a twirl to each particle. 
	offset.x += sin(lifetime * gl_InstanceID / 10);
	offset.y += cos(lifetime * gl_InstanceID / 10);

	//Set the position in space.
    gl_Position = projection * view * model * vec4(aPos + offset, 1.0); 

	//Changes the Colors based on the lifetime of the particle.
	vsColor.y += cos(lifetime * gl_InstanceID / 10);

	//Set the color.
    vertexColor = vec4(vsColor, lifetime - 0.25); 
}
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
//layout (location = 1) in vec3 aColor;
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform float lifetime; 
uniform vec3 offsets[1000];
uniform vec3 pColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	vec3 offset = offsets[gl_InstanceID];
    gl_Position = projection * view * model * vec4(aPos + offset, 1.0); 
	//gl_Position = vec4(aPos + offset, 1.0); 
    vertexColor = vec4(pColor, lifetime - 0.25); 
}
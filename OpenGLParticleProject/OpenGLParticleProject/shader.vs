#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;

//out vec3 ourPos;
out vec3 ourColor;
out vec2 TexCoord;

uniform float xOffset;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//HelloShaders VS
	//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //Inverse Triangle - gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
	//Uniform X-Offset - gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);
    //ourColor = aColor;
	//ourPos = aPos;

	//HelloTextures VS
	//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);

	//HelloTransforms VS
	//gl_Position = transform * vec4(aPos, 1.0f);
	//ourColor = aColor;
	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);

	//HelloTransforms VS part 2
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	ourColor = aColor;
}
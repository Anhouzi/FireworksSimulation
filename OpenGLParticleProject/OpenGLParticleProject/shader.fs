#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
//in vec3 ourPos;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = vec4(ourColor, 1.0);
	//FragColor = vec4(ourPos, 1.0);

	//At 0.2, we are taking 80% of the first texture combined with 20% of the second texture.
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(ourColor, 1.0);
}
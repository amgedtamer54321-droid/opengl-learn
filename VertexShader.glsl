#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexture;
uniform mat4 model;
uniform mat4 view;
uniform mat4 pre;




out vec3 FColor;
out vec2 FTexture;


void main()
	{

gl_Position = pre * view * model * vec4(aPos, 1.0);

FTexture = aTexture;

}

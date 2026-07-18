#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 pre;
out vec3 oNormal; 
out vec3 FragPos;
void main()
	{

gl_Position = pre * view * model * vec4(aPos, 1.0);
FragPos = vec3(model * vec4(aPos, 1.0));
oNormal = normal;
}

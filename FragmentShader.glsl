#version 330 core
out vec4 FragColor;

in vec3 FColor;
in vec2 FTexture;



uniform float color;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1,FTexture),texture(texture2,FTexture),0.5);
}
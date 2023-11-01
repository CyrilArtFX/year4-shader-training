#version 330 core

out vec4 FragColor;

uniform vec3 emissive;

void main()
{
	FragColor = vec4(emissive, 1.0f);
}
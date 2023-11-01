#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 tFragPos;
out vec3 tNormal;
out vec2 tTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

void main()
{
	gl_Position = vec4(aPos, 1.0f) * model * view * projection;
	tFragPos = vec3(vec4(aPos, 1.0f) * model);

	tNormal = aNormal * mat3(normalMatrix);
	
	tTexCoord = aTexCoord;
}
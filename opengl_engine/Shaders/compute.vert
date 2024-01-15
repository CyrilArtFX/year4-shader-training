#version 450 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

uniform vec3 position;
uniform float scale;

void main()
{
    gl_Position = vec4(aPos, 1.0f) * scale + vec4(position.xy, 0.0f, 1.0f); // see how we directly give a vec3 to vec4's constructor
}
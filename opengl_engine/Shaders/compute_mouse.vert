#version 450 core
layout (location = 0) in vec3 aPos;

uniform float scale;
uniform vec3 mousePos;

void main()
{
    gl_Position = vec4(aPos, 1.0f) * scale + vec4(mousePos.xy, 0.0f, 1.0f); // see how we directly give a vec3 to vec4's constructor
}
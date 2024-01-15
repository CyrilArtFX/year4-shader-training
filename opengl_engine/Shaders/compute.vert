#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 squarePos;

uniform float scale;

void main()
{
    gl_Position = vec4(aPos, 1.0f) * scale + vec4(squarePos.xy, 0.0f, 1.0f); // see how we directly give a vec3 to vec4's constructor
}
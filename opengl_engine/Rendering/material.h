#pragma once

#include "shader.h"

class Material
{
public:
	Material(Shader& shaderUsed) : shader(shaderUsed) {}
	virtual ~Material() {}

	virtual void use() = 0;

	Shader& getShader() { return shader; }

protected:
	Shader& shader;
};
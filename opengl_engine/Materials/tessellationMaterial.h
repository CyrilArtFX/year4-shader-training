#pragma once

#include <Rendering/material.h>

class TessellationMaterial : public Material
{
public:
	TessellationMaterial(Shader& shaderUsed);

	void use() override;

private:

};


#pragma once

#include <Rendering/material.h>
#include <Maths/Vector3.h>

class FlatEmissiveMaterial : public Material
{
public:
	FlatEmissiveMaterial(Shader& shaderUsed, Vector3 emissiveColor_ = Vector3::one);

	void use() override;

	void setEmissiveColor(Vector3 emissiveColor_);

private:
	Vector3 emissiveColor;
};


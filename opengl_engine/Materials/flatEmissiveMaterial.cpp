#include "flatEmissiveMaterial.h"

FlatEmissiveMaterial::FlatEmissiveMaterial(Shader& shaderUsed, Vector3 emissiveColor_)
	: emissiveColor(emissiveColor_), Material(shaderUsed)
{}

void FlatEmissiveMaterial::use()
{
	shader.setVec3("emissive", emissiveColor);
}


void FlatEmissiveMaterial::setEmissiveColor(Vector3 emissiveColor_)
{
	emissiveColor = emissiveColor_;
}

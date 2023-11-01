#pragma once

#include <Rendering/material.h>
#include <Rendering/texture.h>

class LitMaterialDirLight : public Material
{
public:
	LitMaterialDirLight(Shader& shaderUsed, std::weak_ptr<Texture> diffuseMap_, std::weak_ptr<Texture> specularMap_, float shininess_ = 32.0f);

	void use() override;

	void setDiffuseMap(std::weak_ptr<Texture> diffuseMap_);
	void setSpecularMap(std::weak_ptr<Texture> specularMap_);
	void setShininess(float shininess_);

private:
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<Texture> specularMap;
	float shininess;
};
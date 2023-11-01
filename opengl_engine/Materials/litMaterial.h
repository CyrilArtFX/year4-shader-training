#pragma once

#include <Rendering/material.h>
#include <Rendering/texture.h>

class LitMaterial : public Material
{
public:
	LitMaterial(Shader& shaderUsed, std::weak_ptr<Texture> diffuseMap_, std::weak_ptr<Texture> specularMap_, 
		std::weak_ptr<Texture> emissiveMap_ = std::make_shared<Texture>("Resources/Default/black.png"), float shininess_ = 32.0f);

	void use() override;

	void setDiffuseMap(std::weak_ptr<Texture> diffuseMap_);
	void setSpecularMap(std::weak_ptr<Texture> specularMap_);
	void setEmissiveMap(std::weak_ptr<Texture> emissiveMap_);
	void setShininess(float shininess_);

private:
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<Texture> specularMap;
	std::shared_ptr<Texture> emissiveMap;
	float shininess;
};
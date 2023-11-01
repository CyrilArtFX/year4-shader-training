#include "litMaterialDirLight.h"

LitMaterialDirLight::LitMaterialDirLight(Shader& shaderUsed, std::weak_ptr<Texture> diffuseMap_, std::weak_ptr<Texture> specularMap_, float shininess_)
	: diffuseMap(diffuseMap_.lock()), specularMap(specularMap_.lock()), shininess(shininess_), Material(shaderUsed)
{}


//  currently hardcoded for each material (and for each shader)
void LitMaterialDirLight::use()
{
	glActiveTexture(GL_TEXTURE0);
	diffuseMap->use();

	glActiveTexture(GL_TEXTURE1);
	specularMap->use();

	shader.setFloat("material.shininess", shininess);
}


void LitMaterialDirLight::setDiffuseMap(std::weak_ptr<Texture> diffuseMap_)
{
	diffuseMap = diffuseMap_.lock();
}

void LitMaterialDirLight::setSpecularMap(std::weak_ptr<Texture> specularMap_)
{
	specularMap = specularMap_.lock();
}

void LitMaterialDirLight::setShininess(float shininess_)
{
	shininess = shininess_;
}
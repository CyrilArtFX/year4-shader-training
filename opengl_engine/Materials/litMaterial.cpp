#include "litMaterial.h"

LitMaterial::LitMaterial(Shader& shaderUsed, std::weak_ptr<Texture> diffuseMap_, std::weak_ptr<Texture> specularMap_, std::weak_ptr<Texture> emissiveMap_, float shininess_)
	: diffuseMap(diffuseMap_.lock()), specularMap(specularMap_.lock()), emissiveMap(emissiveMap_.lock()), shininess(shininess_), Material(shaderUsed)
{}


//  currently hardcoded for each material (and for each shader)
void LitMaterial::use()
{
	glActiveTexture(GL_TEXTURE0);
	diffuseMap->use();

	glActiveTexture(GL_TEXTURE1);
	specularMap->use();

	glActiveTexture(GL_TEXTURE2);
	emissiveMap->use();

	shader.setFloat("material.shininess", shininess);
}


void LitMaterial::setDiffuseMap(std::weak_ptr<Texture> diffuseMap_)
{
	diffuseMap = diffuseMap_.lock();
}

void LitMaterial::setSpecularMap(std::weak_ptr<Texture> specularMap_)
{
	specularMap = specularMap_.lock();
}

void LitMaterial::setEmissiveMap(std::weak_ptr<Texture> emissiveMap_)
{
	emissiveMap = emissiveMap_.lock();
}

void LitMaterial::setShininess(float shininess_)
{
	shininess = shininess_;
}
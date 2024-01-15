#pragma once

#include <Maths/Matrix4.h>

#include <Rendering/vertexArray.h>
#include <Rendering/material.h>
#include <memory>



class Object
{
public:
	Object(std::weak_ptr<Material> material_, std::weak_ptr<Material> secondaryMaterial_, const float* vertices, unsigned int nbVertices, const unsigned int* indices = 0, unsigned int nbIndices = 0);

	void draw(bool drawForTessellation = false);
	void deleteObject();

	void setMaterial(std::weak_ptr<Material> newMat);

	void setPosition(Vector3 newPos);
	void setScale(Vector3 newScale);
	void setScale(float newUniformScale);

	void TriggerChangeMaterial(bool triggerSecondaryMat);

private:
	void computeMatrix();

	VertexArray vertexArray;

	std::shared_ptr<Material> material;
	std::shared_ptr<Material> primaryMaterial;
	std::shared_ptr<Material> secondaryMaterial;

	Vector3 position{ Vector3::zero };
	Vector3 scale{ Vector3::one };
	Matrix4 modelMatrix{ Matrix4::identity };
	Matrix4 normalMatrix{ Matrix4::identity };
};


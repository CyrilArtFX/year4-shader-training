#include "CubeObject.h"
#include "../engine/Shader.h"
#include "CubeMesh.h"
#include "../engine/Timer.h"

CubeObject::CubeObject(float xP, float yP, CubeMesh* cubeMeshP)
: cubeMesh { cubeMeshP }
{
  setPosition(xP, yP);
}

void CubeObject::update() {
    const float t = Timer::getTimeSinceStart() * 0.3f;
    transform = Matrix4::createTranslation(Vector3(0.0f, 0.0f, -10.0f))
        * Matrix4::createRotationY(t * Maths::toRadians(45.0f))
        * Matrix4::createRotationX(t * Maths::toRadians(21.0f))
        * Matrix4::createTranslation(Vector3(Maths::sin(2.1f * t) * 2.0f, Maths::cos(1.7f * t) * 2.0f, Maths::sin(1.3f * t) * Maths::cos(1.5f * t) * 2.0f));
}

void CubeObject::draw(Shader& shader) {
  shader.setMatrix4("mv_matrix", transform);
  cubeMesh->draw();
}

void CubeObject::setPosition(float xP, float yP) {
  x = xP;
  y = yP;
  transform = computeTransform();
}

Matrix4 CubeObject::computeTransform() {
  return Matrix4::createTranslation(Vector3(x, y, -4.0f));
}
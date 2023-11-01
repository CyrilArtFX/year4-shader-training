#pragma once

#include <glad/glad.h>
#include <Maths/Matrix4.h>


enum Camera_Movement
{
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};


//  default values
const Vector3 POSITION = Vector3::zero;
const Vector3 FRONT = Vector3::unitZ;
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

const float PITCH_MIN = -89.0f;
const float PITCH_MAX = 89.0f;
const float FOV_MIN = 1.0f;
const float FOV_MAX = 45.0f;


class Camera
{
public:
	Camera(Vector3 position_ = POSITION, Vector3 front_ = FRONT, float yaw_ = YAW, float pitch_ = PITCH, float fov_ = FOV);
	Camera() = delete;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	Matrix4 GetViewMatrix();
	
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseScroll(float yoffset);


	void setPosition(Vector3 newPos);

	Vector3 getPosition() { return position; }
	Vector3 getFront() { return front; }
	Vector3 getUp() { return up; }
	Vector3 getRight() { return right; }

	void setSpeed(float newSpeed);
	void setSensitivity(float newSensitivity);

	void setPitchClamp(float min, float max);
	void setFovClamp(float min, float max);

	float getFov() { return fov; }


private:
	void updateCameraVectors();

	Vector3 position;
	Vector3 front;
	Vector3 up;
	Vector3 right;

	float yaw;
	float pitch;
	float camSpeed;
	float sensitivity;
	float fov;

	float pitchMin;
	float pitchMax;
	float fovMin;
	float fovMax;
};


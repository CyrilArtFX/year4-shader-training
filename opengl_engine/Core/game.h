#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include <Rendering/camera.h>
#include <Rendering/texture.h>

#include <Maths/Matrix4.h>
#include <Maths/Vector3.h>
#include <Utils/Color.h>

#include <string>
#include <iostream>
#pragma once

class Game
{
public:
	Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	bool initialize(int wndw_width = 1024, int wndw_height = 720, std::string wndw_name = "Shader Training", bool wndw_capturemouse = true);
	void run();
	void close();



	//  functions redirected
	void windowResize(GLFWwindow* glWindow, int width, int height);
	void processMouse(GLFWwindow* glWindow, double xpos, double ypos);
	void processScroll(GLFWwindow* glWindow, double xoffset, double yoffset);

	void processInput(GLFWwindow* glWindow);

private:
	//  window
	std::unique_ptr<Window> window;

	//  time
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//  camera
	std::unique_ptr<Camera> camera;


	//  mouse
	bool firstMouse = true;
	float lastX = 0;
	float lastY = 0;



	//  light demo
	bool rgbTrigger{ false };
	bool rgbActivated{ false };
	Color lightColor{ Color::white };
	float lightRotationFactor{ 0.3f };

	//  tessellation demo
	bool tessellationTrigger{ false };
	bool tessellationActivated{ false };
	bool tessLevelTrigger{ false };
	int tessLevelInner{ 3 };
	int tessLevelOuter{ 3 };
};


#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include <Rendering/camera.h>
#include <Rendering/texture.h>

#include <Maths/Matrix4.h>
#include <Maths/Vector3.h>
#include <Utils/Color.h>

#include <string>


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
	float windowWidth = 0.0f;
	float windowHeight = 0.0f;

	//  time
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//  mouse
	bool firstMouse = true;
	float lastX = 0.0f;
	float lastY = 0.0f;

	float mouseXPos = 0.0f;
	float mouseYPos = 0.0f;
};


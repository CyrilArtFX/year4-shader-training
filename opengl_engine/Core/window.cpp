#include "window.h"

Window::Window(int width, int height, std::string windowName, bool mouseCaptured)
{
	//  initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window_width = width;
	window_height = height;

	//  create GLFW window
	window = glfwCreateWindow(window_width, window_height, windowName.c_str(), NULL, NULL);

	glfwMakeContextCurrent(window);

	//  capture mouse
	if (mouseCaptured)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Window::changeSize(int width, int height)
{
	window_width = width;
	window_height = height;
}

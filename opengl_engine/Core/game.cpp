#include "game.h"

#include <Rendering/shader.h>
#include <ComputeShader/ComputeShader.h>

#include <Utils/Random.h>

#include <iostream>


constexpr int WORKGROUP_SIZE = 32;
constexpr int NUM_WORKGROUPS = 16;
constexpr int FLOCK_SIZE = (NUM_WORKGROUPS * WORKGROUP_SIZE);

struct FlockMember
{
	Vector3 position;
	unsigned int : 32;
	Vector3 velocity;
	unsigned int : 32;
};


Game::Game()
{
}


bool Game::initialize(int wndw_width, int wndw_height, std::string wndw_name, bool wndw_capturemouse)
{
	//  create window and initialize glfw
	window = std::make_unique<Window>(wndw_width, wndw_height, wndw_name, wndw_capturemouse);
	windowWidth = wndw_width;
	windowHeight = wndw_height;

	GLFWwindow* glWindow = window->getGLFWwindow();
	if (glWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}


	glfwSetWindowUserPointer(glWindow, this);

	glfwSetFramebufferSizeCallback(glWindow, [](GLFWwindow* window, int width, int height)
		{
			auto self = static_cast<Game*>(glfwGetWindowUserPointer(window));
			self->windowResize(window, width, height);
		}
	); //  link window resize callback function

	glfwSetCursorPosCallback(glWindow, [](GLFWwindow* window, double xpos, double ypos)
		{
			auto self = static_cast<Game*>(glfwGetWindowUserPointer(window));
			self->processMouse(window, xpos, ypos);
		}
	); //  link mouse pos callback function

	glfwSetScrollCallback(glWindow, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			auto self = static_cast<Game*>(glfwGetWindowUserPointer(window));
			self->processScroll(window, xoffset, yoffset);
		}
	); //  link mouse scroll callback function



	//  initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	//  configure global OpenGL properties
	glEnable(GL_DEPTH_TEST);


	lastX = wndw_width / 2;
	lastY = wndw_height / 2;


	return true;
}


void Game::run()
{
	// ================
	//  initialization
	// ================

	//  shaders
	Shader squareShader("Shaders/compute.vert", "Shaders/compute.frag");
	Shader squareMouseShader("Shaders/compute_mouse.vert", "Shaders/compute.frag");
	ComputeShader squareComputeShader("Shaders/compute.glsl");

	//  square vertices data
	float squareVertices[] =
	{
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f
	};


	//	buffers for the mouse square
	// ------------------------------
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); //  reset


	//  buffers for the flock
	// -----------------------
	unsigned int flockBuffer[2];
	unsigned int flockRenderVao[2];
	unsigned int geometryBuffer;
	unsigned int frameIndex = 0;

	glGenBuffers(2, flockBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, flockBuffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, FLOCK_SIZE * sizeof(FlockMember), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, flockBuffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, FLOCK_SIZE * sizeof(FlockMember), NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &geometryBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometryBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glGenVertexArrays(2, flockRenderVao); 

	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(flockRenderVao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, geometryBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, flockBuffer[i]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FlockMember), NULL);
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, flockBuffer[0]);
	FlockMember* ptr = reinterpret_cast<FlockMember*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, FLOCK_SIZE * sizeof(FlockMember),GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

	for (int i = 0; i < FLOCK_SIZE; i++)
	{
		ptr[i].position = (Vector3(Random::randomFloat(), Random::randomFloat(), 0.0f) - Vector3(0.5f, 0.5f, 0.0f)) * 2.0f;
		ptr[i].velocity = Vector3::zero;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);



	// ===========
	//  main loop
	// ===========
	while (!glfwWindowShouldClose(window->getGLFWwindow()))
	{
		//  time logic
		// ------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//  inputs part
		// -------------
		processInput(window->getGLFWwindow());


		//  update part
		// -------------
		squareComputeShader.use();

		squareComputeShader.setVec3("repulsion", mouseXPos, mouseYPos, 0.0f);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, flockBuffer[frameIndex]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, flockBuffer[frameIndex ^ 1]);

		glDispatchCompute(NUM_WORKGROUPS, 1, 1);


		//  rendering part
		// ----------------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //  clear window with flat color


		//  mouse square part
		squareMouseShader.use();
		squareMouseShader.setFloat("scale", 0.02f);
		squareMouseShader.setVec3("mousePos", mouseXPos / windowWidth, mouseYPos / windowHeight, 0.0f);
		squareMouseShader.setVec3("color", 1.0f, 0.0f, 0.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//  computed squares part
		squareShader.use();
		squareShader.setFloat("scale", 0.02f);
		squareShader.setVec3("color", 0.0f, 0.0f, 1.0f);

		glBindVertexArray(flockRenderVao[frameIndex]);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, FLOCK_SIZE);


		//  events and buffer swap
		glfwSwapBuffers(window->getGLFWwindow());
		glfwPollEvents();



		//  actualise frame index
		frameIndex ^= 1;
	}


	// ================
	//  end of program
	// ================

	//  delete all resources that are not necessary anymore
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	squareShader.deleteProgram();
	squareMouseShader.deleteProgram();
	squareComputeShader.deleteProgram();
}


void Game::close()
{
	//  properly clear GLFW before closing app
	glfwTerminate();
}





void Game::processInput(GLFWwindow* glWindow)
{
	//  close window when escape is pressed
	if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(glWindow, true);
	}
}


//  callback functions
void Game::windowResize(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height); //  resize OpenGL viewport when GLFW is resized
	window->changeSize(width, height);
	windowWidth = width;
	windowHeight = height;
}

void Game::processMouse(GLFWwindow* glWindow, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = lastX - xpos;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	mouseXPos = Maths::clamp(mouseXPos - xoffset, -windowWidth, windowWidth);
	mouseYPos = Maths::clamp(mouseYPos + yoffset, -windowHeight, windowHeight);
}

void Game::processScroll(GLFWwindow* glWindow, double xoffset, double yoffset)
{
}

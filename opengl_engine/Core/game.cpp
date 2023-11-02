#include "game.h"

#include <Rendering/shader.h>
#include <Objects/object.h>

#include <Materials/litMaterialDirLight.h>
#include <Materials/tessellationMaterial.h>


Game::Game()
{
}


bool Game::initialize(int wndw_width, int wndw_height, std::string wndw_name, bool wndw_capturemouse)
{
	//  create window and initialize glfw
	window = std::make_unique<Window>(wndw_width, wndw_height, wndw_name, wndw_capturemouse);

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
	//  run initialization

	camera = std::make_unique<Camera>(Vector3{ 0.0f, 0.0f, -3.0f });


	//  build and compile shaders
	Shader litObjectShaderDirLight("Shaders/object_lit.vert", "Shaders/object_lit_dirlight.frag");
	Shader tessellationShader("Shaders/tessellation.vert", "Shaders/tessellation.frag");

	//  manually set the textures unit on the shader (need to be done only once)
	litObjectShaderDirLight.use(); //  activate the shader on which you want to set the texture unit before doing it
	litObjectShaderDirLight.setInt("material.diffuse", 0);
	litObjectShaderDirLight.setInt("material.specular", 1);


	//  create textures
	std::shared_ptr<Texture> container_diffuse = std::make_shared<Texture>("Resources/container2.png", GL_RGBA, false);
	std::shared_ptr<Texture> container_specular = std::make_shared<Texture>("Resources/container2_specular.png", GL_RGBA, false);



	//  cube vertices data
	float cubeVertices[] = {
		// positions           // normals           // texture coords
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
		
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
	};

	std::shared_ptr<LitMaterialDirLight> containerMat = std::make_shared<LitMaterialDirLight>(litObjectShaderDirLight, container_diffuse, container_specular);
	std::shared_ptr<TessellationMaterial> tesselationMat = std::make_shared<TessellationMaterial>(tessellationShader);

	Object cube_1(containerMat, tesselationMat, cubeVertices, 36);
	Object cube_2(containerMat, tesselationMat, cubeVertices, 36);
	Object cube_3(containerMat, tesselationMat, cubeVertices, 36);

	cube_1.setPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	cube_2.setPosition(Vector3{ 2.0f, 1.5f, 2.0f });
	cube_3.setPosition(Vector3{ 2.0f, -1.0f, -1.0f });




	//  main loop
	while (!glfwWindowShouldClose(window->getGLFWwindow()))
	{
		//  time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		//  inputs part
		// -------------
		processInput(window->getGLFWwindow());


		//  rendering part
		// ----------------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //  clear window with flat color



		float timeValue = glfwGetTime();

		if (rgbActivated)
		{
			lightColor.x = sin(timeValue * 2.0f);
			lightColor.y = sin(timeValue * 0.7f);
			lightColor.z = sin(timeValue * 1.3f);
		}
		else
		{
			lightColor = Vector3{ 1.0f, 1.0f, 1.0f };
		}

		Vector3 lightDirection = Vector3{ Maths::cos(lightRotationFactor), -1.0f, Maths::sin(lightRotationFactor) };

		if (tessellationActivated)
		{
			cube_1.TriggerChangeMaterial(true);
			cube_2.TriggerChangeMaterial(true);
			cube_3.TriggerChangeMaterial(true);
		}
		else
		{
			cube_1.TriggerChangeMaterial(false);
			cube_2.TriggerChangeMaterial(false);
			cube_3.TriggerChangeMaterial(false);
		}





		//  draw
		Matrix4 view = camera->GetViewMatrix();
		Matrix4 projection = Matrix4::createPerspectiveFOV(Maths::toRadians(camera->getFov()), window->getWidth(), window->getHeigth(), 0.1f, 100.0f);


		if (tessellationActivated)
		{
			tessellationShader.use();

			tessellationShader.setMatrix4("view", view.getAsFloatPtr());
			tessellationShader.setMatrix4("projection", projection.getAsFloatPtr());
		}
		else
		{
			litObjectShaderDirLight.use();

			litObjectShaderDirLight.setVec3("light.ambient", lightColor * 0.1f);
			litObjectShaderDirLight.setVec3("light.diffuse", lightColor * 0.7f);
			litObjectShaderDirLight.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			litObjectShaderDirLight.setVec3("light.direction", lightDirection);

			litObjectShaderDirLight.setVec3("viewPos", camera->getPosition());

			litObjectShaderDirLight.setMatrix4("view", view.getAsFloatPtr());
			litObjectShaderDirLight.setMatrix4("projection", projection.getAsFloatPtr());
		}


		cube_1.draw();
		cube_2.draw();
		cube_3.draw();



		//  events and buffer swap
		glfwSwapBuffers(window->getGLFWwindow());
		glfwPollEvents();
	}


	//  delete all resources that are not necessary anymore
	cube_1.deleteObject();
	cube_2.deleteObject();
	cube_3.deleteObject();
	litObjectShaderDirLight.deleteProgram();
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

	//  move camera
	if (glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(Forward, deltaTime);

	if (glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(Backward, deltaTime);

	if (glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(Left, deltaTime);

	if (glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(Right, deltaTime);

	if (glfwGetKey(glWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(Up, deltaTime);

	if (glfwGetKey(glWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->ProcessKeyboard(Down, deltaTime);


	//  trigger rgb
	if (glfwGetKey(glWindow, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
	{
		if(!rgbTrigger) rgbActivated = !rgbActivated;
		rgbTrigger = true;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_SEMICOLON) == GLFW_RELEASE)
	{
		rgbTrigger = false;
	}


	//  rotate light
	if (glfwGetKey(glWindow, GLFW_KEY_P) == GLFW_PRESS)
		lightRotationFactor += 5.0f * deltaTime;


	//  trigger tessellation
	if (glfwGetKey(glWindow, GLFW_KEY_T) == GLFW_PRESS)
	{
		if (!tessellationTrigger) tessellationActivated = !tessellationActivated;
		tessellationTrigger = true;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_T) == GLFW_RELEASE)
	{
		tessellationTrigger = false;
	}


	//  change tessellation inner and outer levels
	if (glfwGetKey(glWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (!tessLevelTrigger) tessLevelInner = Maths::max(tessLevelInner + 1, 10);
		tessLevelTrigger = true;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (!tessLevelTrigger) tessLevelInner = Maths::min(tessLevelInner - 1, 1);
		tessLevelTrigger = true;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (!tessLevelTrigger) tessLevelOuter = Maths::min(tessLevelOuter - 1, 1);
		tessLevelTrigger = true;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (!tessLevelTrigger) tessLevelOuter = Maths::max(tessLevelOuter + 1, 10);
		tessLevelTrigger = true;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_UP) == GLFW_RELEASE && glfwGetKey(glWindow, GLFW_KEY_DOWN) == GLFW_RELEASE &&
		glfwGetKey(glWindow, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(glWindow, GLFW_KEY_RIGHT) == GLFW_RELEASE)
	{
		tessLevelTrigger = false;
	}
}


//  callback functions
void Game::windowResize(GLFWwindow* glWindow, int width, int height)
{
	glViewport(0, 0, width, height); //  resize OpenGL viewport when GLFW is resized
	window->changeSize(width, height);
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

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void Game::processScroll(GLFWwindow* glWindow, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(float(yoffset));
}

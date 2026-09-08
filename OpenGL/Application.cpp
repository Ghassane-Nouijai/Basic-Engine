#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include "Scene.h"
#include "PhysicsWorld.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "SimObject.h"
#include "PhysicsObject.h"

#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "InputProcesses.h"

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	
	// Set ups

	Camera camera(glm::vec3(0.0f, 5.0f, 25.0f));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	InputProcesses inputs;

	glfwMakeContextCurrent(window);
	inputs.SetCallbacks(window, camera);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	{
		Shader      shader("Material.shader");
		Renderer    renderer;
		Scene       scene;          // handles drawing
		PhysicsWorld world;  // handles physics

		// Create objects
		// Two balls falling and colliding
		

		auto obj1 = std::make_shared<SimObject>(
			std::make_unique<Sphere>(0.2f, 50),
			PhysicsObject(glm::vec3(0.0f, 5.0f, 0.0f), 1.0f, 0.2f, 0.8f, false, glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
			Material::Rubber()
		);

		auto obj2 = std::make_shared<SimObject>(
			std::make_unique<Sphere>(0.5f, 50),
			PhysicsObject(glm::vec3(0.0f, 3.0f, 0.0f), 10.0f, 0.5f, 0.8f, false, glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
			Material::Rubber()
		);

		glm::quat groundOrientation = glm::rotation(
			glm::vec3(0.0f, 0.0f, 1.0f),  
			glm::vec3(0.0f, 1.0f, 0.0f)    
		);

		auto ground = std::make_shared<SimObject>(
			std::make_unique<Rectangle>(100.0f, 100.0f),
			PhysicsObject(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f,
				glm::vec3(50.0f, 0.2f, 50.0f), 1.0f, true,
				groundOrientation),  
			Material::Gold()
		);

		scene.Add(obj1);  world.AddPhysicsObject(obj1);
		scene.Add(obj2);  world.AddPhysicsObject(obj2);
		scene.Add(ground);  world.AddPhysicsObject(ground);

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();
			shader.Bind();

			shader.SetUniform3fv("u_Light.position", glm::vec3(8 * sin(glfwGetTime()), 5.0f, 8 * cos(glfwGetTime())));
			shader.SetUniform3fv("u_Light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
			shader.SetUniform3fv("u_Light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			shader.SetUniform3fv("u_Light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			shader.SetUniform3fv("u_ViewPos", camera.Position);

			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			deltaTime = std::min(deltaTime, 1.0f / 30.0f);

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			shader.SetUniformMat4("projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			shader.SetUniformMat4("view", view);

			//glm::vec3 position = glm::vec3(sin(3 * glfwGetTime()), cos(3 * glfwGetTime()), 0.0f);

			inputs.processInput(window, camera, deltaTime);

			world.Update(deltaTime);

			scene.Draw(shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
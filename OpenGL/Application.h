#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "Camera.h"
#include "InputProcesses.h"
#include "PhysicsObject.h"
#include "PhysicsWorld.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "SimObject.h"
#include "Sphere.h"

class Application
{
public:
	Application() = default;
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	~Application()
	{
		Shutdown();
	}

	void Run()
	{
		InitializeWindow();

		Camera camera(glm::vec3(0.0f, 5.0f, 25.0f));
		InputProcesses inputs;

		glfwMakeContextCurrent(m_Window);
		inputs.SetCallbacks(m_Window, camera);
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}

		glEnable(GL_DEPTH_TEST);

		Shader shader("Material.shader");
		Renderer renderer;
		Scene scene;
		PhysicsWorld world;

		CreateScene(scene, world);

		float lastFrame = 0.0f;
		while (!glfwWindowShouldClose(m_Window))
		{
			renderer.Clear();
			shader.Bind();

			shader.SetUniform3fv("u_Light.position", glm::vec3(
				8.0f * std::sin(static_cast<float>(glfwGetTime())),
				5.0f,
				8.0f * std::cos(static_cast<float>(glfwGetTime()))));
			shader.SetUniform3fv("u_Light.ambient", glm::vec3(1.0f));
			shader.SetUniform3fv("u_Light.diffuse", glm::vec3(0.8f));
			shader.SetUniform3fv("u_Light.specular", glm::vec3(1.0f));
			shader.SetUniform3fv("u_ViewPos", camera.Position);

			const float currentFrame = static_cast<float>(glfwGetTime());
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			deltaTime = std::min(deltaTime, 1.0f / 30.0f);

			const glm::mat4 projection = glm::perspective(
				glm::radians(camera.Zoom),
				static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight),
				0.1f,
				100.0f);
			shader.SetUniformMat4("projection", projection);
			shader.SetUniformMat4("view", camera.GetViewMatrix());

			inputs.processInput(m_Window, camera, deltaTime);
			world.Update(deltaTime);
			scene.Draw(shader);

			glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}
	}

private:
	static constexpr unsigned int ScreenWidth = 1200;
	static constexpr unsigned int ScreenHeight = 900;

	GLFWwindow* m_Window = nullptr;
	bool m_GlfwInitialized = false;

	void InitializeWindow()
	{
		if (glfwInit() == GLFW_FALSE)
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}
		m_GlfwInitialized = true;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(
			ScreenWidth, ScreenHeight, "LearnOpenGL", nullptr, nullptr);
		if (m_Window == nullptr)
		{
			throw std::runtime_error("Failed to create GLFW window");
		}
	}

	static void CreateScene(Scene& scene, PhysicsWorld& world)
	{
		auto obj1 = std::make_shared<SimObject>(
			std::make_unique<Sphere>(0.2f, 50),
			PhysicsObject(glm::vec3(0.0f, 5.0f, 0.0f), 1.0f, 0.2f, 0.8f, false,
				glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
			Material::Rubber());

		auto obj2 = std::make_shared<SimObject>(
			std::make_unique<Sphere>(0.5f, 50),
			PhysicsObject(glm::vec3(0.0f, 3.0f, 0.0f), 10.0f, 0.5f, 0.8f, false,
				glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
			Material::Rubber());

		const glm::quat groundOrientation = glm::rotation(
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		auto ground = std::make_shared<SimObject>(
			std::make_unique<Rectangle>(100.0f, 100.0f),
			PhysicsObject(glm::vec3(0.0f), 0.0f, glm::vec3(50.0f, 0.2f, 50.0f),
				1.0f, true, groundOrientation),
			Material::Gold());

		scene.Add(obj1);
		world.AddPhysicsObject(obj1);
		scene.Add(obj2);
		world.AddPhysicsObject(obj2);
		scene.Add(ground);
		world.AddPhysicsObject(ground);
	}

	void Shutdown() noexcept
	{
		if (m_Window != nullptr)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}

		if (m_GlfwInitialized)
		{
			glfwTerminate();
			m_GlfwInitialized = false;
		}
	}
};
#pragma once

#include "glew.h"
#include <glfw3.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

#include "Camera.h"
#include "Skybox.h"
#include "Terrain.h"

class Renderer
{
public:
	float _deltaTime;
	float _prevTime;
	float _aspectRation;
	float _zoom = 10000;
	glm::vec3 _focusPoint;
	glm::vec3 _cameraRotation = glm::vec3(45.0f, 45.0f, 0.0f);

	Skybox* _skyboxPtr;
	Terrain _terrain;
	Camera _camera;


	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		Renderer::FrameBufferSizeCallback(window, width, height);
	}

	void FrameBufferSizeCallbackImpl(GLFWwindow* window, int width, int height)
	{
		_camera.FrameBufferSizeCallback(window, width, height);
	}

	void init(GLFWwindow* window)
	{
		const char* skyboxImages[6] = { "right.png", "left.png", "top.png", "bottom.png", "front.png", "back.png" };
		_skyboxPtr = new Skybox("./models/cube.obj", "./textures/skybox", skyboxImages);
		_camera.Fov = 100000;
		glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

		glEnable(GL_DEPTH_TEST);

		_terrain.InitTerrain(30);
		_terrain.LoadFromFile("./map/N49E020.hgt");

		int centerPoint = (int)_terrain.GetSize() / 2;
		_focusPoint = _terrain.GetPosition(centerPoint, centerPoint);
	}

	void calculateDeltaTime()
	{
		float time = glfwGetTime();
		_deltaTime = time - _prevTime;
		_prevTime = time;
	}

	glm::vec3 DirectionTo(glm::vec3 from, glm::vec3 to)
	{
		glm::vec3 direction = to - from;
		direction = glm::normalize(direction);
		return direction;
	}

	glm::vec3 RotateAroundAxis(glm::vec3 vector3, float angel, glm::vec3 axis)
	{
		float angleRadians = glm::radians(angel);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, axis);
		glm::vec4 rotatedVector = rotationMatrix * glm::vec4(vector3, 1.0f);
		return glm::vec3(rotatedVector);
	}

	glm::vec3 RotateAround(glm::vec3 rotation)
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

		position = RotateAroundAxis(position, rotation.x, xAxis);
		position = RotateAroundAxis(position, rotation.y, yAxis);
		position = RotateAroundAxis(position, rotation.z, zAxis);

		return position;
	}

	void processInput(GLFWwindow* window)
	{
		float rotationSpeed = 0.05f * _deltaTime * _terrain.GetWorldScale();
		float zoomSpeed = 1.0f * _deltaTime * _terrain.GetWorldScale();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			_cameraRotation.x -= rotationSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			_cameraRotation.x += rotationSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			_cameraRotation.y -= rotationSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			_cameraRotation.y += rotationSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			_zoom -= zoomSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			_zoom += zoomSpeed;
		}

		if (_zoom < 0)
		{
			_zoom = 0;
		}

		if (_cameraRotation.x <= -360 || _cameraRotation.x >= 360)
		{
			_cameraRotation.x = 0;
		}

		if (_cameraRotation.y >= 360 || _cameraRotation.y <= -360)
		{
			_cameraRotation.y = 0;
		}

		_camera.CameraPos = _focusPoint + (RotateAround(_cameraRotation) * _zoom);
		_camera.CameraDir = DirectionTo(_camera.CameraPos, _focusPoint);
	}

	void renderScene(GLFWwindow* window)
	{
		_skyboxPtr->Render(&_camera);
		_terrain.Render(&_camera);
	}

	void renderLoop(GLFWwindow* window)
	{
		while (!glfwWindowShouldClose(window))
		{
			calculateDeltaTime();
			processInput(window);
			renderScene(window);
			glfwPollEvents();
		}
	}

	void shutdown(GLFWwindow* window)
	{
		_skyboxPtr->Destroy();
		_terrain.Destroy();
	}

} renderer;

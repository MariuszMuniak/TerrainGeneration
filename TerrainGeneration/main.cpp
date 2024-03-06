#include "glew.h"

#include <glfw3.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <iostream>
#include <cmath>

#include "Renderer.hpp"

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 1920;
	int height = 1080;

	GLFWwindow* window = glfwCreateWindow(width, height, "Terrain Generator", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewInit();
	glViewport(0, 0, width, height);

	renderer.init(window);

	renderer.renderLoop(window);

	renderer.shutdown(window);
	glfwTerminate();

	return 0;
}
#ifndef CAMERA_H
#define CAMERA_H

#include <glfw3.h>
#include "glm/glm.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	glm::vec3 CameraPos;
	glm::vec3 CameraDir;
	float Fov = 1000;

	glm::mat4 GetViewProjectionMatrix();
	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	float GetAspectRatio() const { return _aspectRatio; }
private:
	float _aspectRatio;

	glm::mat4 CreateCameraMatrix();
	glm::mat4 CreatePerspectiveMatrix();
};

#endif

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	
}

Camera::~Camera()
{
	
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
	return CreatePerspectiveMatrix() * CreateCameraMatrix();
}

void Camera::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	_aspectRatio = width / (float)height;
	glViewport(0, 0, width, height);
}

glm::mat4 Camera::CreatePerspectiveMatrix()
{
	glm::mat4 perspectiveMatrix;
	float n = 0.05;
	float a1 = glm::min(_aspectRatio, 1.f);
	float a2 = glm::min(1 / _aspectRatio, 1.f);
	perspectiveMatrix = glm::mat4({
		1,0.,0.,0.,
		0.,_aspectRatio,0.,0.,
		0.,0.,(Fov + n) / (n - Fov),2 * Fov * n / (n - Fov),
		0.,0.,-1.,0.,
		});


	perspectiveMatrix = glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}

glm::mat4 Camera::CreateCameraMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(CameraDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide, CameraDir));
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-CameraDir.x,-CameraDir.y,-CameraDir.z,0,
		0.,0.,0.,1.,
		});
	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(trans, -CameraPos);

	return cameraMatrix;
}



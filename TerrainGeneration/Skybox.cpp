#include "Skybox.h"

#include <cstring>

#include "glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "ShaderLoader.h"
#include "SOIL/SOIL.h"

Skybox::Skybox(const char* cubeObjFilePath, const char* dirPath, const char* images[6])
{
	_textureId = LoadCubemaps(dirPath, images);
	_programId = ShaderLoader::CreateProgram("shaders/shader_skybox.vert", "shaders/shader_skybox.frag");
	ModelLoader::LoadModelToContext(cubeObjFilePath, _cubeContext);
}

Skybox::~Skybox()
{
	Destroy();
	
}

void Skybox::Destroy()
{
	ShaderLoader::DeleteProgram(_programId);
}


GLuint Skybox::LoadCubemaps(const char* dirPath, const char* images[6])
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width;
	int height;
	for (int i = 0; i < 6; i++)
	{
		int filePathLength = strlen(dirPath) + strlen(images[i]);
		char* filePath = new char[filePathLength];
		strcpy(filePath, dirPath);
		strcat(filePath, images[i]);

		unsigned char* image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureId;
}

void Skybox::Render(Camera* camera)
{
	glDisable(GL_DEPTH_TEST);
	glUseProgram(_programId);
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::translate(trans, camera->CameraPos);
	glm::mat4 transformation = camera->GetViewProjectionMatrix() * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(_programId, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniform1i(glGetUniformLocation(_programId, "skybox"), 0);
	DrawContext(_cubeContext);
	glEnable(GL_DEPTH_TEST);
}

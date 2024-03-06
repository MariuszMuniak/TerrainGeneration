#pragma once

#include <glew.h>
#include "Camera.h"
#include "ModelLoader.h"

class Skybox
{
public:
	Skybox(const char* cubeObjFilePath, const char* dirPath, const char* images[6]);
	~Skybox();
	void Destroy();
	void Render(Camera* camera);
private:
	GLuint _textureId;
	GLuint _programId;
	RenderContext _cubeContext;
	GLuint LoadCubemaps(const char* dirPath, const char* images[6]);
};


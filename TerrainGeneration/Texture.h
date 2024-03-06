#pragma once

#include <glew.h>

class Texture
{
public:
	static  GLuint LoadTexture(const char* filePath);
	static  void SetActiveTexture(GLuint textureId, const char* shaderVariableName, GLuint programId, int textureUint);
};


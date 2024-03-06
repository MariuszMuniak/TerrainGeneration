#include "Texture.h"

#include "glew.h"
#include "SOIL/SOIL.h"


GLuint Texture::LoadTexture(const char* filePath)
{
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int width;
	int height;
	unsigned char* image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	return id;
}

void Texture::SetActiveTexture(GLuint textureId, const char* shaderVariableName, GLuint programId, int textureUint)
{
	glUniform1i(glGetUniformLocation(programId, shaderVariableName), textureUint);
	glActiveTexture(GL_TEXTURE0 + textureUint);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

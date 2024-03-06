#pragma once
#include <glew.h>
#include <iostream>

class ShaderLoader
{
public:
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	static void DeleteProgram(GLuint program);
private:
	static std::string ReadShader(const char* fileName);
	static GLuint CreateShader(GLenum shaderType, std::string source, const char* shaderName);
};


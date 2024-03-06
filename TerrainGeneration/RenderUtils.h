#pragma once

#include <algorithm>

#include <glew.h>
#include "objload.h"
#include "freeglut.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct RenderContext
{
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint vertexIndexBuffer;
	int size = 0;

	void initFromOBJ(obj::Model& model);

	void initFromAssimpMesh(aiMesh* mesh);
};

void DrawContext(RenderContext& context);

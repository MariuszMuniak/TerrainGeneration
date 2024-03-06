#ifndef TRIANGLE_LIST_H
#define TRIANGLE_LIST_H

#include <glew.h>
#include <vector>
#include "glm/glm.hpp"

class Terrain;

class TriangleList
{
public:
	TriangleList();
	~TriangleList();
	void Destroy();
	void CreateTriangleList(int width, int depth, const Terrain* pTerrain);
	void Render();
private:
	struct Vertex
	{
		glm::vec3 Pos;
		glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec2 Tex;

		void InitVertex(const Terrain* pTerrain, int x, int z);
	};

	int _width;
	int _depth;
	GLuint _vao;
	GLuint _vbo;
	GLuint _ib;

	void CreateGLState();
	void PopulateBuffers(const Terrain* pTerrain);
	void InitVertices(const Terrain* pTerrain, std::vector<Vertex>& Vertices);
	void InitIndices(std::vector<unsigned int>& Indices);
	void CalcNormals(std::vector<Vertex>& Vertices, std::vector<unsigned int>& Indices);
};
#endif
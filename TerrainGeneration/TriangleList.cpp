#include "TriangleList.h"

#include "Terrain.h"

TriangleList::TriangleList()
{
	
}

TriangleList::~TriangleList()
{
	Destroy();
}

void TriangleList::Destroy()
{
	if (_vao > 0) {
		glDeleteVertexArrays(1, &_vao);
	}

	if (_vbo > 0) {
		glDeleteBuffers(1, &_vbo);
	}

	if (_ib > 0) {
		glDeleteBuffers(1, &_ib);
	}
}

void TriangleList::CreateTriangleList(int width, int depth, const Terrain* pTerrain)
{
	_width = width;
	_depth = depth;

	CreateGLState();
	PopulateBuffers(pTerrain);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TriangleList::CreateGLState()
{
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glGenBuffers(1, &_ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ib);

	int POS_LOC = 0;
	int NORMAL_LOC = 1;
	int TEX_LOC = 2;

	size_t NumFloats = 0;

	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NumFloats * sizeof(float)));
	NumFloats += 3;

	glEnableVertexAttribArray(NORMAL_LOC);
	glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NumFloats * sizeof(float)));
	NumFloats += 3;

	glEnableVertexAttribArray(TEX_LOC);
	glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NumFloats * sizeof(float)));
	NumFloats += 2;
}

void TriangleList::PopulateBuffers(const Terrain* pTerrain)
{
	std::vector<Vertex> Vertices;
	Vertices.resize(_width * _depth);

	InitVertices(pTerrain, Vertices);

	std::vector<unsigned int> Indices;
	int NumQuads = (_width - 1) * (_depth- 1);
	Indices.resize(NumQuads * 6);
	InitIndices(Indices);

	CalcNormals(Vertices, Indices);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}


void TriangleList::InitVertices(const Terrain* pTerrain, std::vector<Vertex>& Vertices)
{
	int Index = 0;
	for (int z = 0; z < _depth; z++)
	{
		for (int x = 0; x < _width; x++)
		{
			assert(Index < Vertices.size());
			Vertices[Index].InitVertex(pTerrain, x, z);
			Index++;
		}
	}
}

void TriangleList::Vertex::InitVertex(const Terrain* pTerrain, int x, int z)
{
	float y = pTerrain->GetHeight(x, z);
	Pos = glm::vec3(x * pTerrain->GetWorldScale(), y, z * pTerrain->GetWorldScale());
	Pos.x *= -1;
	Pos.z *= -1;
	float size = pTerrain->GetSize();
	Tex = glm::vec2(static_cast<float>(x) / size, static_cast<float>(z) / size);
}


void TriangleList::InitIndices(std::vector<unsigned>& Indices)
{
	int Index = 0;
	for (int z = 0; z < _depth - 1; z++)
	{
		for (int x = 0; x < _width - 1; x++)
		{
			unsigned int IndexBottomLeft = z * _width + x;
			unsigned int IndexTopLeft = (z + 1) * _width + x;
			unsigned int IndexTopRight = (z + 1) * _width + x + 1;
			unsigned int IndexBottomRight = z * _width + x + 1;

			Indices[Index++] = IndexBottomLeft;
			Indices[Index++] = IndexBottomRight;
			Indices[Index++] = IndexTopRight;

			Indices[Index++] = IndexTopRight;
			Indices[Index++] = IndexTopLeft;
			Indices[Index++] = IndexBottomLeft;
		}
	}
}

void TriangleList::CalcNormals(std::vector<Vertex>& Vertices, std::vector<unsigned>& Indices)
{
	for (unsigned int i = 0; i < Indices.size(); i += 3)
	{
		unsigned int Index0 = Indices[i];
		unsigned int Index1 = Indices[i + 1];
		unsigned int Index2 = Indices[i + 2];
		glm::vec3 v1 = Vertices[Index1].Pos - Vertices[Index0].Pos;
		glm::vec3 v2 = Vertices[Index2].Pos - Vertices[Index0].Pos;
		glm::vec3 normal = cross(v2, v1);
		normal = glm::normalize(normal);

		Vertices[Index0].Normal += normal;
		Vertices[Index1].Normal += normal;
		Vertices[Index2].Normal += normal;
	}

	for (unsigned int i = 0; i < Vertices.size(); i++)
	{
		Vertices[i].Normal = glm::normalize(Vertices[i].Normal);
	}
}


void TriangleList::Render()
{
	glBindVertexArray(_vao);

	glDrawElements(GL_TRIANGLES, (_depth - 1) * (_width - 1) * 6, GL_UNSIGNED_INT, NULL);
	
	glBindVertexArray(0);
}


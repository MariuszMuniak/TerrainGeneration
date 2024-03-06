#ifndef TERRAIN_H
#define TERRAIN_H

#include "Array2d.h"
#include "Camera.h"
#include "TriangleList.h"

class Terrain
{
public:
	Terrain();
	~Terrain();
	void Destroy();
	void InitTerrain(float worldScale);
	void Render(Camera* camera);
	void LoadFromFile(const char* filePath);
	float GetHeight(int x, int z) const { return _heightMap.Get(x, z); }
	glm::vec3 GetPosition(int x, int z) const
	{
		return glm::vec3(x * _worldScale, GetHeight(x, z), z * _worldScale);
	}
	float GetWorldScale() const { return _worldScale; }
	float GetSize() const { return _terrainSize; }
private:
	float _worldScale = 1.0f;
	int _terrainSize = 0;
	Array2D<float> _heightMap;
	TriangleList* _triangleList;
	GLuint _program;
	GLuint _texture;

	void LoadHeightMapFile(const char* filePath);
};
#endif
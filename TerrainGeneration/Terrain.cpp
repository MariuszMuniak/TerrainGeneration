#include "Terrain.h"

#include <fstream>
#include <iostream>
#include <string>

#include "ShaderLoader.h"
#include "SunLight.h"
#include "Texture.h"

const int SRTM_SIZE = 3601;

Terrain::Terrain()
{
	_triangleList = new TriangleList();
}

Terrain::~Terrain()
{
	Destroy();
}

void Terrain::Destroy()
{
	_heightMap.Destroy();
	_triangleList->Destroy();
	ShaderLoader::DeleteProgram(_program);
}

void Terrain::InitTerrain(float worldScale)
{
	_worldScale = worldScale;
	_program = ShaderLoader::CreateProgram("shaders/shader_terrain.vert", "shaders/shader_terrain.frag");
	_texture = Texture::LoadTexture("./textures/clipped_exp_2.png");
}

void Terrain::Render(Camera* camera)
{
	glUseProgram(_program);
	glm::mat4 modelMatrix = glm::mat4();
	glm::mat4 transformation = camera->GetViewProjectionMatrix() * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(_program, "gVP"), 1, GL_FALSE, (GLfloat*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(_program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	glm::vec3 lightColor = SunLight::GetColor();
	glm::vec3 lightDir = SunLight::GetDirection();
	glm::vec3 cameraPos = camera->CameraPos;
	glUniform3f(glGetUniformLocation(_program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(_program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform3f(glGetUniformLocation(_program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	Texture::SetActiveTexture(_texture, "colorTexture", _program, 0);

	_triangleList->Render();
}


void Terrain::LoadFromFile(const char* filePath)
{
	LoadHeightMapFile(filePath);
}

void Terrain::LoadHeightMapFile(const char* filePath)
{
	_terrainSize = SRTM_SIZE;
	_heightMap.InitArray2D(_terrainSize, _terrainSize);
	std::ifstream file(filePath, std::ios::in | std::ios::binary);
	if (!file)
	{
		std::cout << "Error opening file!" << std::endl;
	}

	unsigned char buffer[2];
	for (int i = 0; i < _terrainSize; ++i)
	{
		for (int j = 0; j < _terrainSize; ++j)
		{
			if (!file.read(reinterpret_cast<char*>(buffer), sizeof(buffer)))
			{
				std::cout << "Error reading file!" << std::endl;
			}

			_heightMap.Set(j, i, (buffer[0] << 8) | buffer[1]);
		}
	}
}



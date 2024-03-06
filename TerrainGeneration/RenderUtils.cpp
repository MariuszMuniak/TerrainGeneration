#include "RenderUtils.h"

void RenderContext::initFromAssimpMesh(aiMesh* mesh)
{
	vertexArray = 0;
	vertexBuffer = 0;
	vertexIndexBuffer = 0;

	std::vector<float> textureCord;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		if (mesh->mTextureCoords[0] != nullptr)
		{
			textureCord.push_back(mesh->mTextureCoords[0][i].x);
			textureCord.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			textureCord.push_back(0.0f);
			textureCord.push_back(0.0f);
		}
	}

	if (mesh->mTextureCoords[0] == nullptr)
	{
		std::cout << "no uv cords\n";
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j< face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	unsigned int vertexDataBufferSize = sizeof(float) * mesh->mNumVertices * 3;
	unsigned int vertexNormalBufferSize = sizeof(float) * mesh->mNumVertices * 3;
	unsigned int vertexTexBufferSize = sizeof(float) * mesh->mNumVertices * 2;
	unsigned int vertexTangentBufferSize = sizeof(float) * mesh->mNumVertices * 3;
	unsigned int vertexBiTangentBufferSize = sizeof(float) * mesh->mNumVertices * 3;

	unsigned int vertexElementBufferSize = sizeof(unsigned int) * indices.size();
	size = indices.size();

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexElementBufferSize, &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBufferData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize + vertexBiTangentBufferSize, NULL, GL_STATIC_DRAW);

	unsigned int offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertexDataBufferSize, mesh->mVertices);
	offset += vertexDataBufferSize;
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertexNormalBufferSize, mesh->mNormals);
	offset += vertexNormalBufferSize;
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertexTexBufferSize, &textureCord[0]);
	offset += vertexTexBufferSize;
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertexTangentBufferSize, mesh->mTangents);
	offset += vertexTangentBufferSize;
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertexBiTangentBufferSize, mesh->mBitangents);

	glVertexAttribPointer(0, 3, GL_FLAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)vertexDataBufferSize);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertexNormalBufferSize + vertexDataBufferSize));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize));
}

void DrawContext(RenderContext& context)
{
	glBindVertexArray(context.vertexArray);
	glDrawElements(GL_TRIANGLES, context.size, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

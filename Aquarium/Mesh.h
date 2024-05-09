#pragma once
#include <memory>
#include <vector>
#include "Shader.h"
#include "Vertex.h"
#include "ObjectTexture.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,const std::vector<ObjectTexture>& textures);
	Mesh(unsigned int numVertices, const std::shared_ptr <Vertex>& vertices, unsigned int numIndexes, const std::shared_ptr <unsigned int>& indices, const std::vector<ObjectTexture>& textures);
	void Draw(const Shader& shader);

private:
	void SetupMesh();
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int numVertices;
	std::shared_ptr<Vertex> vertices;
	unsigned int numIndexes;
	std::shared_ptr <unsigned int> indices;
	std::vector<ObjectTexture> m_textures;
};

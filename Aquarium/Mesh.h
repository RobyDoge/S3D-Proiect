#pragma once
#include <memory>

#include "Shader.h"
#include "Textures.h"
#include "Vertex.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,const Textures& textures);
	Mesh(unsigned int numVertices, const std::shared_ptr <Vertex>& vertices, unsigned int numIndexes, const std::shared_ptr <unsigned int>& indices, const Textures& textures);
	void Draw(const Shader& shader);

private:
	void setupMesh();
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int numVertices;
	std::shared_ptr<Vertex> vertices;
	unsigned int numIndexes;
	std::shared_ptr <unsigned int> indices;
	Textures m_textures;
};

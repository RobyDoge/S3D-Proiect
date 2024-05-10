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
	void Draw(const Shader& shader) const;

private:
	void SetupMesh();
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ebo;
	unsigned int m_numVertices;
	std::shared_ptr<Vertex> m_vertices;
	unsigned int m_numIndexes;
	std::shared_ptr <unsigned int> m_indices;
	std::vector<ObjectTexture> m_textures;
};

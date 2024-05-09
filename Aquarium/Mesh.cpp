#include "Mesh.h"
#include <glew.h>
#include <ranges>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::vector<ObjectTexture>& textures)
{
	numVertices = vertices.size();
	this->vertices.reset(new Vertex[numVertices]);

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		this->vertices.get()[i] = vertices[i];
	}

	numIndexes = indices.size();
	this->indices.reset(new unsigned int[numIndexes]);
	for (size_t i = 0; i < indices.size(); ++i)
	{
		this->indices.get()[i] = indices[i];
	}
	this->m_textures = textures;

	SetupMesh();
}

Mesh::Mesh(unsigned int numVertices, const std::shared_ptr<Vertex>& vertices, unsigned int numIndexes, 
	const std::shared_ptr<unsigned int>& indices, const std::vector<ObjectTexture>& textures) :
	numVertices(numVertices),
	vertices(vertices),
	numIndexes(numIndexes),
	indices(indices),
	m_textures(textures)
{
	SetupMesh();
}


void Mesh::Draw(const Shader& shader)
{
	// bind appropriate m_textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	unsigned int index{0};
	for (const auto& texture : m_textures)
	{
		glActiveTexture(GL_TEXTURE0 + index); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = texture.type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.GetId(), (name + number).c_str()), index);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, texture.id);
		index++;
	}

	// draw mesh
	glBindVertexArray(VAO);

	//std::cout << "draw triangles: " << numIndexes << std::endl;
	glDrawElements(GL_TRIANGLES, numIndexes, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), &vertices.get()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * sizeof(unsigned int), &indices.get()[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Tangent)));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Bitangent)));
	// ids
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_BoneIDs)));

	// weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_Weights)));
	glBindVertexArray(0);

}

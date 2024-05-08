#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public:
    Model(string const& path, bool bSmoothNormals, bool gamma = false);
    void Draw(Shader& shader);

private:
    void LoadModel(string const& path, bool bSmoothNormals);
    void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Textures LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

private:
	Textures m_textures;
	std::vector<Mesh> m_meshes;
	string m_directory;
	bool m_gammaCorrection;

};

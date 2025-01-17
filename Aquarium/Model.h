﻿#pragma once
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public:
	Model()= default;
    Model(std::string const& path, bool bSmoothNormals, bool gamma = false);
    void Draw(const Shader& shader);

private:
    void LoadModel(std::string const& path, bool smoothNormals);
    void ProcessNode(const aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<ObjectTexture> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);
	unsigned int TextureFromFile(const char* path, bool gamma = false) const;
private:
	std::vector<ObjectTexture> m_textures;
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	bool m_gammaCorrection;

};
